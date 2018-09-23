// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Props/RHObstacle.h"
#include "RHObstacleSlowDown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRH_API ARHObstacleSlowDown : public ARHObstacle
{
	GENERATED_BODY()
	

protected:
	void ApplyCollisionEffect() override;

	UFUNCTION()
	void OnEndSlowDownEffect();

	void PlaySlowDownEffect();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySlowDownEffect();
	
protected:
	// Based on character movement speed
	UPROPERTY(EditAnywhere, Category = "RHObstacleSlowDown", meta = (ClampMax = 0.8f, ClampMin = 0.1f))
	float SlowingFactor = 0.25f;

	UPROPERTY(EditAnywhere, Category = "RHObstacleSlowDown", meta = (ClampMax = 10.f, ClampMin = 1.f))
	float SlowDownDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "RHObstacleSlowDown")
	UParticleSystem* ParticleEffect;

	FTimerHandle TimerHandle_SlowDownCharacter;
	
};
