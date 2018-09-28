// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHGameplayAbility.h"
#include "RHAbilitySystemComponent.h"
#include "RHTargetType.h"
#include "ProjectRHCharacter.h"

URHGameplayAbility::URHGameplayAbility() {}

FRHGameplayEffectContainerSpec URHGameplayAbility::MakeEffectContainerSpecFromContainer(const FRHGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	// First figure out our actor info
	FRHGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	AProjectRHCharacter* OwningCharacter = Cast<AProjectRHCharacter>(OwningActor);
	URHAbilitySystemComponent* OwningASC = URHAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		// If we have a target type, run the targeting logic. This is optional, targets can be added later
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const URHTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		// If we don't have an override level, use the default ont he ability system component
		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OwningASC->GetDefaultAbilityLevel();
		}

		// Build GameplayEffectSpecs for each applied effect
		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FRHGameplayEffectContainerSpec URHGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FRHGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);

	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}
	return FRHGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> URHGameplayAbility::ApplyEffectContainerSpec(const FRHGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;
	
	// Iterate list of effect specs and apply them to their target data
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));	
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> URHGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, FGameplayAbilityTargetDataHandle& TargetData, int32 OverrideGameplayLevel)
{
	FRHGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	TargetData = Spec.TargetData;
	return ApplyEffectContainerSpec(Spec);
}



