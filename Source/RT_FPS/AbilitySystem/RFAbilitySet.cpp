// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFAbilitySet.h"
#include "AbilitySystemComponent.h"

URFAbilitySet::URFAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URFAbilitySet::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject)
{
	for (const FGameplayAbilityInfo& AbilityInfo : Abilities)
	{
		
		FGameplayAbilitySpec AbilitySpec(AbilityInfo.GameplayAbilityClass, AbilityInfo.AbilityLevel, InputID);
		//All Ability has source object that instance of weapon, if null -> not weapon ability
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityInfo.InputTag);

		if (AbilityInfo.GameplayAbilityClass)
		{
			ActivatableAbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(AbilitySpec));
		}
	}
}

void URFAbilitySet::RemoveAbilities(UAbilitySystemComponent* AbilitySystemComponent)
{
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : ActivatableAbilitySpecHandles)
	{
		AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
	}

	ActivatableAbilitySpecHandles.Empty();
}
