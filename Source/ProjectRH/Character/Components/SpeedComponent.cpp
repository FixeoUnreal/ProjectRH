// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "SpeedComponent.h"
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "TimerManager.h"
#include <UnrealNetwork.h>
#include <Engine/World.h>

// Sets default values for this component's properties
USpeedComponent::USpeedComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...

	SetIsReplicated(true);
}


// Called when the game starts
void USpeedComponent::BeginPlay()
{
	Super::BeginPlay();

	// Initializing replicated variables
	OwningCharacter = Cast<ACharacter>(GetOwner());
	if(!ensure(OwningCharacter)){ return; }
	MovementComp = OwningCharacter->GetCharacterMovement();
	if(!ensure(MovementComp)){ return; }
	BaseWalkSpeed = MovementComp->MaxWalkSpeed;
}


void USpeedComponent::EndSpeedEffect()
{
	// Reset speed
	if(!ensure(MovementComp)){ return; }
	MovementComp->MaxWalkSpeed = BaseWalkSpeed;

	UWorld* World = GetWorld();
	if (!ensure(World)) { return; }
	World->GetTimerManager().ClearTimer(TimerHandle_WalkSpeedEffect);
}

// Called every frame
void USpeedComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USpeedComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USpeedComponent, BaseWalkSpeed);
	DOREPLIFETIME(USpeedComponent, OwningCharacter);
	DOREPLIFETIME(USpeedComponent, MovementComp);
}

void USpeedComponent::MulticastApplySpeedEffect_Implementation(float WalkSpeedFactor /*= 0.25f*/, float Duration /*= 2.5f*/)
{
	if(!ensure(MovementComp)){ return; }

	float AcceptableWalkSpeedFactor = FMath::Clamp(WalkSpeedFactor, MinWalkSpeedFactor, MaxWalkSpeedFactor);
	float AcceptableDuration = FMath::Clamp(Duration, MinDuration, MaxDuration);
	MovementComp->MaxWalkSpeed *= AcceptableWalkSpeedFactor;

	// End effect after the duration
	UWorld* World = GetWorld();
	if(!ensure(World)){ return; }
	World->GetTimerManager().SetTimer(TimerHandle_WalkSpeedEffect, this, &USpeedComponent::EndSpeedEffect, AcceptableDuration);
	UE_LOG(LogTemp, Warning, TEXT("After Apply speed effect"));
}

