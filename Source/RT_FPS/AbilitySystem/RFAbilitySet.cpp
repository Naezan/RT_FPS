// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFAbilitySet.h"
#include "AbilitySystemComponent.h"

URFAbilitySet::URFAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URFAbilitySet::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent) const
{
	for (const FGameplayAbilityInfo& AbilityInfo : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityInfo.GameplayAbilityClass, AbilityInfo.AbilityLevel);
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityInfo.InputTag);

		if (AbilityInfo.GameplayAbilityClass)
		{
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}
