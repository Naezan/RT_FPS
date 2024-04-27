// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "RFAbilitySet.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;
class UGameplayEffect;
struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;

USTRUCT()
struct FGameplayAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;

	UPROPERTY(EditDefaultsOnly, Category = BindInfo)
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS(BlueprintType)
class RT_FPS_API URFAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	URFAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveAbilities(UAbilitySystemComponent* AbilitySystemComponent, UObject* SourceObject);
	void RemoveAbilities(UAbilitySystemComponent* AbilitySystemComponent);

public:
	UPROPERTY(EditDefaultsOnly, Category = AbilitySet)
	TArray<FGameplayAbilityInfo> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = AbilitySet)
	TArray<TSubclassOf<UGameplayEffect>> Attributes;

private:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> ActivatableAbilitySpecHandles;
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActivatableEffectHandles;
};
