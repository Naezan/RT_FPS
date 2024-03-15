// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RFAbilityInputData.generated.h"

class URFAbilityInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS(BlueprintType, Const)
class RT_FPS_API URFAbilityInputData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	const URFAbilityInputAction* FindAbilityInputActionByTag(FGameplayTag InInputTag);
	const TArray<URFAbilityInputAction*> GetAllInputActions();
	const TMap<FGameplayTag, URFAbilityInputAction*>& GetAbilityInputInfo() const;
	const UInputMappingContext* GetAbilityInputMapping() const;
	// Find AbilityInputID using GameplayTag, returning INDEX_NONE if fail to found
	int32 GetInputIDByTag(FGameplayTag InInputTag);
	
private:
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess="true"))
	TMap<FGameplayTag, URFAbilityInputAction*> AbilityActionInputs;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> AbilityInputMapping;

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("AbilityInput", GetFName());
	}
};
