// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#pragma once

#include "Abilities/GameplayAbilityTypes.h"
#include "RHAbilityTypes.h"
#include "RHTargetType.generated.h"

class AProjectRHCharacter;
class AActor;
struct FGameplayEventData;

/**
 * Class that is used to determine targeting for abilities
 * It is meant to be blueprinted to run target logic
 * This does not subclass GameplayAbilityTargetActor because this class is never instanced into the world
 * This can be used as a basis for a game-specific targeting blueprint
 * If your targeting is more complicated you may need to instance into the world once or as a pooled actor
 */
UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class PROJECTRH_API URHTargetType : public UObject
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	URHTargetType() {}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
		void GetTargets(AProjectRHCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
};

/** Trivial target type that uses the owner */
UCLASS(NotBlueprintable)
class PROJECTRH_API URHTargetType_UseOwner : public URHTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	URHTargetType_UseOwner() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AProjectRHCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

/** Trivial target type that pulls the target out of the event data */
UCLASS(NotBlueprintable)
class PROJECTRH_API URHTargetType_UseEventData : public URHTargetType
{
	GENERATED_BODY()

public:
	// Constructor and overrides
	URHTargetType_UseEventData() {}

	/** Uses the passed in event data */
	virtual void GetTargets_Implementation(AProjectRHCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

