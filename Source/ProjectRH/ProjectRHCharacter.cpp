// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ProjectRHCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Pickup/SHPowerUp.h"
#include <UnrealNetwork.h>
#include <Components/BoxComponent.h>
#include "ProjectRH.h"
#include "RHPlayerState.h"
#include "Terrain/WayGate.h"
#include "TimerManager.h"
#include "Character/Components/SpeedComponent.h"
#include <AbilitySystemComponent.h>


//////////////////////////////////////////////////////////////////////////
// AProjectRHCharacter

const int32 Ability_Input_ID_NormalSkill = 0;
const int32 Ability_Input_ID_UltimateSkill = 1;
const int32 Ability_Input_ID_PassiveSkill = INDEX_NONE;

AProjectRHCharacter::AProjectRHCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create attackable zone
	AttackZone = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackZone"));
	AttackZone->SetupAttachment(RootComponent);
	AttackZone->SetRelativeLocation(FVector(650, 0, 0));
	AttackZone->SetBoxExtent(FVector(600, 500, 100));
	AttackZone->SetCollisionProfileName(TEXT("AttackZone"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(CC_ATTACKZONE, ECR_Overlap);

	// Initialize speed component
	SpeedComp = CreateDefaultSubobject<USpeedComponent>(TEXT("SpeedComp"));

	// Initialize ability system
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystem"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectRHCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("UsePowerUp", IE_Pressed, this, &AProjectRHCharacter::ServerActivatePowerUp);

	PlayerInputComponent->BindAction("PCResetMoveForward", IE_Released, this, &AProjectRHCharacter::ResetMoveForwardValue);
	PlayerInputComponent->BindAction("PCResetMoveRight", IE_Released, this, &AProjectRHCharacter::ResetMoveRightValue);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProjectRHCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProjectRHCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProjectRHCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProjectRHCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AProjectRHCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AProjectRHCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProjectRHCharacter::OnResetVR);

	// Setup input for ability system
	AbilitySystem->BindAbilityActivationToInputComponent(
		PlayerInputComponent, 
		FGameplayAbiliyInputBinds("ConfirmInput", "CancelInput", "AbilityInput")
	);
}


void AProjectRHCharacter::ActivatePowerUp()
{
	if(Role < ROLE_Authority || bPowerUpActivated)
	{ 
		return;
	}
	if (PowerUpInstance)
	{
		PowerUpInstance->Activate();
		bPowerUpActivated = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PowerUpInstance was not set. PowerUp can not be activated."));
	}
}

void AProjectRHCharacter::ForceMoveForward()
{
	MoveForward(1.f);
}

void AProjectRHCharacter::ResetMoveForwardValue()
{
	MoveForwardValue = 0.f;
}

void AProjectRHCharacter::ResetMoveRightValue()
{
	MoveRightValue = 0.f;
}

void AProjectRHCharacter::UpdateDistanceToNextWayGate()
{
	if(!PlayerState){ return; }
	ARHPlayerState* RHPlayerState = Cast<ARHPlayerState>(PlayerState);
	if(!ensure(RHPlayerState)){ return; }
	AWayGate* NextWaveGate = RHPlayerState->GetNexWayGate();
	if(!ensure(NextWaveGate)){ return; }

	// Calculate the positive distance 
	float ToNextWaveGateDistance = FMath::Abs(FVector::PointPlaneDist(
		GetActorLocation(),
		NextWaveGate->GetActorLocation(),
		NextWaveGate->GetActorForwardVector()
	));

	RHPlayerState->SetDistanceToNextWayGate(ToNextWaveGateDistance);
}

void AProjectRHCharacter::ServerActivatePowerUp_Implementation()
{
	ActivatePowerUp();
}

bool AProjectRHCharacter::ServerActivatePowerUp_Validate()
{
	return true;
}

void AProjectRHCharacter::BeginPlay()
{
	Super::BeginPlay();
	DesiredRotation = GetActorRotation();

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if(!ensure(MovementComp)){ return; }
	BaseWalkSpeed = MovementComp->MaxWalkSpeed;

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			TimerHandle_UpdateDistanceToNextWaveGate,
			this,
			&AProjectRHCharacter::UpdateDistanceToNextWayGate,
			0.5f,
			true
		);
	}

	// Initializing abilities
	if (AbilitySystem)
	{
		if (HasAuthority())
		{
			if (NormalAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(NormalAbility.GetDefaultObject(), 1, Ability_Input_ID_NormalSkill));
			}
			if (UltimateAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(UltimateAbility.GetDefaultObject(), 1, Ability_Input_ID_UltimateSkill));
			}
			if (PassiveAbility)
			{
				AbilitySystem->GiveAbility(FGameplayAbilitySpec(PassiveAbility.GetDefaultObject(), 1, Ability_Input_ID_PassiveSkill));
			}
		}
		AbilitySystem->InitAbilityActorInfo(this, this);
	}
	
}

void AProjectRHCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_UpdateDistanceToNextWaveGate);
	}
}

void AProjectRHCharacter::SetPowerUp(ASHPowerUp* PowerUpToSet)
{
	// Clear unused PowerUp. No need to call after activation because it will destroy itself
	if (PowerUpInstance && !bPowerUpActivated)
	{
		PowerUpInstance->Destroy();
	}

	PowerUpInstance = PowerUpToSet;
	bPowerUpActivated = false;
}

UBoxComponent* AProjectRHCharacter::GetAttackZone() const
{
	return AttackZone;
}

void AProjectRHCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AProjectRHCharacter, PowerUpInstance);
	DOREPLIFETIME(AProjectRHCharacter, bInRunMode);
	DOREPLIFETIME(AProjectRHCharacter, DesiredRotation);
}

USpeedComponent* AProjectRHCharacter::GetSpeedComp() const
{
	return SpeedComp;
}

UAbilitySystemComponent* AProjectRHCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void AProjectRHCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AbilitySystem->RefreshAbilityActorInfo();
}

void AProjectRHCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProjectRHCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AProjectRHCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AProjectRHCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProjectRHCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProjectRHCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		FVector Direction;
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		{
			Direction = DesiredRotation.Vector().ToOrientationQuat().GetUpVector();
		}
		else
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		AddMovementInput(Direction, Value);

		//For moveforward animation
		MoveForwardValue = Value;
	}
}

void AProjectRHCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		FVector Direction;
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		{
			Direction = DesiredRotation.Vector().ToOrientationQuat().GetRightVector();
		}
		else
		{
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
		// add movement in that direction
		AddMovementInput(Direction, Value);

		// For moveright animation
		MoveRightValue = Value;
	}
}
