// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHTargetType.h"
#include "RHGameplayAbility.h"
#include "ProjectRHCharacter.h"

void URHTargetType::GetTargets_Implementation(AProjectRHCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void URHTargetType_UseOwner::GetTargets_Implementation(AProjectRHCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void URHTargetType_UseEventData::GetTargets_Implementation(AProjectRHCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}



