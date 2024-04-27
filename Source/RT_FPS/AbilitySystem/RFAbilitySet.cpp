// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Interface/AbilityInputInterface.h"

URFAbilitySet::URFAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void URFAbilitySet::GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject)
{
	for (const FGameplayAbilityInfo& AbilityInfo : Abilities)
	{
		int32 InputID = INDEX_NONE;
		
		// Only player has InputID
		if (IAbilityInputInterface* OwningCharacter = Cast<IAbilityInputInterface>(AbilitySystemComponent->GetAvatarActor()))
		{
			if (AbilityInfo.InputTag != FGameplayTag::EmptyTag)
			{
				OwningCharacter->ResetInputIDByTag(AbilityInfo.InputTag);
				InputID = OwningCharacter->GetAbilityInputActionIDByTag(AbilityInfo.InputTag);
				OwningCharacter->RegisterInputIDByTag(InputID, AbilityInfo.InputTag);
			}
		}

		FGameplayAbilitySpec AbilitySpec(AbilityInfo.GameplayAbilityClass, AbilityInfo.AbilityLevel, InputID);
		//All Ability has source object that instance of weapon, if null -> not weapon ability
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityInfo.InputTag);

		if (AbilityInfo.GameplayAbilityClass)
		{
			ActivatableAbilitySpecHandles.Add(AbilitySystemComponent->GiveAbility(AbilitySpec));
		}
	}

	for (const TSubclassOf<UGameplayEffect>& Attribute : Attributes)
	{
		const UGameplayEffect* GameplayEffect = Attribute->GetDefaultObject<UGameplayEffect>();

		ActivatableEffectHandles.Add(AbilitySystemComponent->ApplyGameplayEffectToSelf(GameplayEffect, 1.f, AbilitySystemComponent->MakeEffectContext()));
	}
}

void URFAbilitySet::RemoveAbilities(UAbilitySystemComponent* AbilitySystemComponent)
{
	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : ActivatableAbilitySpecHandles)
	{
		AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
	}

	for (const FActiveGameplayEffectHandle& EffectHandle : ActivatableEffectHandles)
	{
		if (EffectHandle.IsValid())
		{
			AbilitySystemComponent->RemoveActiveGameplayEffect(EffectHandle);
		}
	}

	ActivatableAbilitySpecHandles.Empty();
	ActivatableEffectHandles.Empty();
}
