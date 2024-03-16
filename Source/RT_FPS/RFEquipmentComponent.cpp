// Fill out your copyright notice in the Description page of Project Settings.


#include "RFEquipmentComponent.h"
#include "RFWeaponInstance.h"
#include "RFLogMacros.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

URFEquipmentComponent::URFEquipmentComponent()
{

}


// Called when the game starts
void URFEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool URFEquipmentComponent::OnEquip(TSubclassOf<URFWeaponInstance> EquipWeaponInstance)
{
	if (!IsValid(CurrentWeaponInstance))
	{
		if (EquipWeaponInstance == nullptr)
		{
			UE_LOG(LogRF, Warning, TEXT("EquipWeapon Instance is invalid on [%s] : Client or Server [%s]"), *GetNameSafe(this), *GetClientServerContextString(GetOwner()));
			return false;
		}

		CurrentWeaponInstance = NewObject<URFWeaponInstance>(GetOwner(), EquipWeaponInstance);
	}

	CurrentWeaponInstance->EquipWeapon();

	return true;
}

bool URFEquipmentComponent::OnUnEquip()
{
	if (!IsValid(CurrentWeaponInstance))
	{
		UE_LOG(LogRF, Warning, TEXT("Doesn't have equipped weapon."));
		return false;
	}

	CurrentWeaponInstance->UnEquipWeapon();
	CurrentWeaponInstance = nullptr;

	return true;
}

AActor* URFEquipmentComponent::GetReplicatedFPWeapon() const
{
	return CurrentWeaponInstance ? CurrentWeaponInstance->GetFPWeaponActor() : nullptr;
}

AActor* URFEquipmentComponent::GetReplicatedTPWeapon() const
{
	return CurrentWeaponInstance ? CurrentWeaponInstance->GetTPWeaponActor() : nullptr;
}

void URFEquipmentComponent::OnRep_CurrentWeaponInstance()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

bool URFEquipmentComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	
	if (IsValid(CurrentWeaponInstance))
	{
		WroteSomething |= Channel->ReplicateSubobject(CurrentWeaponInstance, *Bunch, *RepFlags);
	}

	return WroteSomething;
}

void URFEquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = ELifetimeRepNotifyCondition::REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentWeaponInstance, SharedParams);
}