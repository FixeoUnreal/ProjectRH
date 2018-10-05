// Copyright 2018 , Hoang Giang Phi, All Rights Reserved.

#include "RHAbilitySystemComponent.h"
#include "RHGameplayAbility.h"
#include "AbilitySystemGlobals.h"
#include "Character/ProjectRHCharacter.h"

URHAbilitySystemComponent::URHAbilitySystemComponent() {}

void URHAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<URHGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	// Iterate the list of all ability specs
	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		// Iterate all instances on this ability spec
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<URHGameplayAbility>(ActiveAbility));
		}
	}
}

int32 URHAbilitySystemComponent::GetDefaultAbilityLevel() const
{
	AProjectRHCharacter* OwningCharacter = Cast<AProjectRHCharacter>(OwnerActor);

	if (OwningCharacter)
	{
		return OwningCharacter->GetCharacterLevel();
	}
	return 1;
}

URHAbilitySystemComponent* URHAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent)
{
	return Cast<URHAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}




