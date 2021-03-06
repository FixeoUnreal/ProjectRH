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
#include "Pickup/RHPowerUp.h"
#include <UnrealNetwork.h>
#include <Components/BoxComponent.h>
#include "ProjectRH.h"
#include "Replication/RHPlayerState.h"
#include "Terrain/WayGate.h"
#include "TimerManager.h"
#include "Character/Components/SpeedComponent.h"
#include "AbilitySystem/RHAbilitySystemComponent.h"
#include "AbilitySystem/RHAttributeSet.h"
#include "Character/RHPlayerController.h"
#include "General/LobbyGameMode.h"
#include <Kismet/GameplayStatics.h>


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
	AbilitySystem = CreateDefaultSubobject<URHAbilitySystemComponent>(TEXT("AbilitySystem"));
	AbilitySystem->SetIsReplicated(true);

	// Create the attribute set, this replicates by default
	AttributeSet = CreateDefaultSubobject<URHAttributeSet>(TEXT("AttributeSet"));

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
		FGameplayAbilityInputBinds("ConfirmInput", "CancelInput", "AbilityInput")
	);
}


void AProjectRHCharacter::ActivatePowerUp()
{
	UE_LOG(LogTemp, Warning, TEXT("activated: %d"), bPowerUpActivated);
	if (Role < ROLE_Authority || bPowerUpActivated)
	{
		return;
	}
	if (PowerUpInstance)
	{
		bPowerUpActivated = true;
		PowerUpInstance->Activate();
		UE_LOG(LogTemp, Warning, TEXT("Powerup activate!"));
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
	if (!PlayerState) { return; }
	ARHPlayerState* RHPlayerState = Cast<ARHPlayerState>(PlayerState);
	if (!RHPlayerState) { return; }
	AWayGate* NextWaveGate = RHPlayerState->GetNexWayGate();
	if (!NextWaveGate) { return; }
	UE_LOG(LogTemp, Warning, TEXT("After nullptr check"));

	// Calculate the positive distance 
	float ToNextWaveGateDistance = FMath::Abs(FVector::PointPlaneDist(
		GetActorLocation(),
		NextWaveGate->GetActorLocation(),
		NextWaveGate->GetActorForwardVector()
	));

	RHPlayerState->SetDistanceToNextWayGate(ToNextWaveGateDistance);
	UE_LOG(LogTemp, Warning, TEXT("%s Distance: %f"), *GetName(), ToNextWaveGateDistance);
}

void AProjectRHCharacter::HandleDamage(float DamageAmount, const FHitResult& HitInfo, const struct FGameplayTagContainer& DamageTags, AProjectRHCharacter* InstigatorCharacter, AActor* DamageCauser)
{

}

void AProjectRHCharacter::HandleHealthChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

}

void AProjectRHCharacter::HandleManaChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{

}

void AProjectRHCharacter::HandleMoveSpeedChanged(float DeltaValue, const struct FGameplayTagContainer& EventTags)
{
	// Update the character movement's walk speed
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed * GetMoveSpeed();
	UE_LOG(LogTemp, Warning, TEXT("Handle movement speed"));
	if (bAbilitiesInitialized)
	{
		OnMoveSpeedChanged(DeltaValue, EventTags);
	}
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

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!ensure(MovementComp)) { return; }
	BaseWalkSpeed = MovementComp->MaxWalkSpeed;

	// No need to update position in lobby
	if (HasAuthority())
	{
		ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(UGameplayStatics::GetGameMode(this));
		if (LobbyGM)
		{
			bInRunMode = false;
			return;
		}
	}

	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Update distance"));
		GetWorldTimerManager().SetTimer(
			TimerHandle_UpdateDistanceToNextWaveGate,
			this,
			&AProjectRHCharacter::UpdateDistanceToNextWayGate,
			0.5f,
			true
		);
	}

	AddStartupAbilities();


}

void AProjectRHCharacter::AddStartupAbilities()
{
	if (!ensure(AbilitySystem)) { return; }
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
	bAbilitiesInitialized = true;
}

void AProjectRHCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (HasAuthority())
	{
		GetWorldTimerManager().ClearTimer(TimerHandle_UpdateDistanceToNextWaveGate);
	}
}

void AProjectRHCharacter::SetPowerUp(ARHPowerUp* PowerUpToSet)
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
	DOREPLIFETIME(AProjectRHCharacter, DesiredForwardVector);
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

int32 AProjectRHCharacter::GetCharacterLevel() const
{
	// TODO if level implemented
	return 1;
}

float AProjectRHCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

void AProjectRHCharacter::SetPowerUpActivated(bool bInPowerUpActivated)
{
	bPowerUpActivated = bInPowerUpActivated;
	UE_LOG(LogTemp, Warning, TEXT("After set activated: %d"), bPowerUpActivated);
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
			//Direction = DesiredRotation.Vector().ToOrientationQuat().GetUpVector();
			Direction = DesiredForwardVector.UpVector;
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
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		FVector Direction;
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Flying)
		{
			//Direction = DesiredRotation.Vector().ToOrientationQuat().GetRightVector();
			Direction = DesiredForwardVector.RightVector;
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
