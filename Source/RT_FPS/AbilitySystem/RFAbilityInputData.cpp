// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFAbilityInputData.h"
#include "Input/RFAbilityInputAction.h"

const URFAbilityInputAction* URFAbilityInputData::FindAbilityInputActionByTag(FGameplayTag InInputTag)
{
	return AbilityActionInputs.FindRef(InInputTag);
}

const TArray<URFAbilityInputAction*> URFAbilityInputData::GetAllInputActions()
{
	TArray<URFAbilityInputAction*> InputActions;
	// Copy
	AbilityActionInputs.GenerateValueArray(InputActions);

	return InputActions;
}

const TMap<FGameplayTag, URFAbilityInputAction*>& URFAbilityInputData::GetAbilityInputInfo() const
{
	return AbilityActionInputs;
}

const UInputMappingContext* URFAbilityInputData::GetAbilityInputMapping() const
{
	return AbilityInputMapping;
}

int32 URFAbilityInputData::GetInputIDByTag(FGameplayTag InInputTag)
{
	if (const URFAbilityInputAction* InputAction = FindAbilityInputActionByTag(InInputTag))
	{
		return InputAction->GetInputID();
	}

	return INDEX_NONE;
}
