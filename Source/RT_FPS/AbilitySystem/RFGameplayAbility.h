// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "RFGameplayAbility.generated.h"

class URFWeaponInstance;

/**
 * 
 */
UCLASS()
class RT_FPS_API URFGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	URFGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Client Only. If you want to replicated property, do not call weaponinstance
	UFUNCTION(BlueprintPure)
	URFWeaponInstance* GetWeaponInstance() const;

	UFUNCTION(BlueprintPure)
	AActor* GetFPReplicatedWeaponActor() const;
	UFUNCTION(BlueprintPure)
	AActor* GetFPReplicatedWeaponActorByActorInfo(const FGameplayAbilityActorInfo& ActorInfo) const;
	UFUNCTION(BlueprintPure)
	AActor* GetTPReplicatedWeaponActor() const;
	UFUNCTION(BlueprintPure)
	AActor* GetTPReplicatedWeaponActorByActorInfo(const FGameplayAbilityActorInfo& ActorInfo) const;

protected:
	//~UGameplayAbility interface
	//virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	//~End of UGameplayAbility interface

protected:
	UPROPERTY(EditDefaultsOnly, Category = Cost)
	FGameplayTag CostTag;
	UPROPERTY(EditDefaultsOnly, Category = Cost)
	FGameplayTag FailCostTag;
};
