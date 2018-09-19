// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHObstacleSlowDown.h"
#include "ProjectRHCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>


void ARHObstacleSlowDown::ApplyCollisionEffect()
{
	if (!ensure(OverlappingCharacter)) { return; }
	CharMovementComp = OverlappingCharacter->GetCharacterMovement();
	if (!ensure(CharMovementComp)) { return; }

	// So character can get back to normal speed after this
	OriginalWalkSpeed = OverlappingCharacter->GetBaseWalkSpeed();

	// Applying slow down effect
	CharMovementComp->MaxWalkSpeed *= SlowingFactor;

	PlaySlowDownEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_SlowDownCharacter, this, &ARHObstacleSlowDown::EndSlowDownCharacter, SlowDownDuration);
}

void ARHObstacleSlowDown::EndSlowDownCharacter()
{
	if (!ensure(CharMovementComp)) { return; }
	CharMovementComp->MaxWalkSpeed = OriginalWalkSpeed;

	GetWorldTimerManager().ClearTimer(TimerHandle_SlowDownCharacter);

	if (bDestroyAfterOverlap)
	{
		Destroy();
	}
}

void ARHObstacleSlowDown::PlaySlowDownEffect()
{
	if(!ensure(ParticleEffect)){ return; }
	UGameplayStatics::SpawnEmitterAtLocation(
		this,
		ParticleEffect,
		GetActorLocation()
	);
}

