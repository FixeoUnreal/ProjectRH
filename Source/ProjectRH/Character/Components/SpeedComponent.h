// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpeedComponent.generated.h"

class UCharacterMovementComponent;
class ACharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTRH_API USpeedComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpeedComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	/**
	/* Description: change the walk speed of the character based on it's base speed and the WalkSpeedFactor
	/* Returns:   void 
	/* Parameter: WalkSpeedFactor - if = 0, character will be stunned. Clamped between Min- and MaxWalkSpeedFactor
	/* Parameter: Duration - Clamped between Min- and MaxDuration
	**/
	UFUNCTION(NetMulticast, Reliable)
	void MulticastApplySpeedEffect(float WalkSpeedFactor = 0.25f, float Duration = 2.5f);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void EndSpeedEffect();

protected:
	UPROPERTY(Replicated)
	float BaseWalkSpeed = 0.f;

	UPROPERTY(Replicated)
	ACharacter* OwningCharacter;

	UPROPERTY(Replicated)
	UCharacterMovementComponent* MovementComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedComponent", meta = (ClampMin = 1.f, ClampMax = 20.f))
	float MaxWalkSpeedFactor = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedComponent", meta = (ClampMin = 0.f, ClampMax = 0.9f))
	float MinWalkSpeedFactor = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedComponent", meta = (ClampMin = 1.f, ClampMax = 10.f))
	float MaxDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SpeedComponent", meta = (ClampMin = 0.f, ClampMax = 0.9f))
	float MinDuration = 0.f;

	FTimerHandle TimerHandle_WalkSpeedEffect;

};
