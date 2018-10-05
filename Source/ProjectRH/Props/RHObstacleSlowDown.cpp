// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHObstacleSlowDown.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "TimerManager.h"
#include <Kismet/GameplayStatics.h>
#include "Character/Components/SpeedComponent.h"
#include "Character/ProjectRHCharacter.h"


void ARHObstacleSlowDown::ApplyCollisionEffect()
{
	if (!ensure(OverlappingCharacter)) { return; }
	USpeedComponent* CharSpeedComp = OverlappingCharacter->GetSpeedComp();
	if(!ensure(CharSpeedComp)){ return; }
	
	CharSpeedComp->MulticastApplySpeedEffect(SlowingFactor, SlowDownDuration);

	MulticastPlaySlowDownEffect();

	GetWorldTimerManager().SetTimer(TimerHandle_SlowDownCharacter, this, &ARHObstacleSlowDown::OnEndSlowDownEffect, SlowDownDuration);
}

void ARHObstacleSlowDown::OnEndSlowDownEffect()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_SlowDownCharacter);
	if (bDestroyAfterOverlap && HasAuthority())
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

void ARHObstacleSlowDown::MulticastPlaySlowDownEffect_Implementation()
{
	PlaySlowDownEffect();
}

