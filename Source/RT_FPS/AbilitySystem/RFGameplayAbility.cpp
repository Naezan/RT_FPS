// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFGameplayAbility.h"
#include "RFWeaponInstance.h"
#include "RFEquipmentComponent.h"

URFGameplayAbility::URFGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

URFWeaponInstance* URFGameplayAbility::GetWeaponInstance() const
{
	return Cast<URFWeaponInstance>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo));
}

AActor* URFGameplayAbility::GetFPReplicatedWeaponActor() const
{
	if (AActor* OwningPawn = GetAvatarActorFromActorInfo())
	{
		URFEquipmentComponent* EquipmentComponent = OwningPawn->FindComponentByClass<URFEquipmentComponent>();
		return EquipmentComponent ? EquipmentComponent->GetReplicatedFPWeapon() : nullptr;
	}
	return nullptr;
}

AActor* URFGameplayAbility::GetTPReplicatedWeaponActor() const
{
	if (AActor* OwningPawn = GetAvatarActorFromActorInfo())
	{
		URFEquipmentComponent* EquipmentComponent = OwningPawn->FindComponentByClass<URFEquipmentComponent>();
		return EquipmentComponent ? EquipmentComponent->GetReplicatedTPWeapon() : nullptr;
	}
	return nullptr;
}

void URFGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		// TODO Process TargetData
	}
	else
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, /* bReplicateEndAbility */ true, /* bWasCancelled */ false);
	}
}
