// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilityInputInterface.generated.h"

class UInputAction;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class RT_FPS_API IAbilityInputInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual const UInputAction* GetAbilityInputActionByTag(FGameplayTag InputTag) const { return nullptr; }
	virtual int32 GetAbilityInputActionIDByTag(FGameplayTag InputTag) const { return INDEX_NONE; }
};
