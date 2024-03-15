// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RFPlayerData.generated.h"

class URFAbilitySet;
class URFWeaponInstance;
class URFAbilityInputData;

/**
 * 
 */
UCLASS(BlueprintType)
class RT_FPS_API URFPlayerData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	URFPlayerData(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<URFAbilitySet> AbilitySet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Input")
	TObjectPtr<URFAbilityInputData> AbilityInputData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<URFWeaponInstance> EquipWeaponInstance;
};
