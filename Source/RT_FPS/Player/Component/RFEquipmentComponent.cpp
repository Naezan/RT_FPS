// Fill out your copyright notice in the Description page of Project Settings.


#include "RFEquipmentComponent.h"
#include "Weapon/RFWeaponInstance.h"
#include "RFLogMacros.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Character.h"
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
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CurrentWeaponInstance, this);
	}

	CurrentWeaponInstance->EquipWeapon();
	InitializeMagazineData();

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

void URFEquipmentComponent::InitializeMagazineData(int32 DefaultMagazineIndex)
{
	if (CurrentWeaponInstance)
	{
		int32 MagazineCapacity = CurrentWeaponInstance->GetMagazineCapacity();
		if (MagazineCapacity > 0)
		{
			CurrentMagazineInfo.CurrentMagazineIndex = DefaultMagazineIndex;
			CurrentMagazineInfo.EquipMagazineAmmo.Init(0, MagazineCapacity);
			for (int32 i = 0; i < MagazineCapacity; ++i)
			{
				CurrentMagazineInfo.EquipMagazineAmmo[i] = CurrentWeaponInstance->GetMagazineInfos()[i].AmmoCount;
			}
			MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CurrentMagazineInfo, this);

			MagazineAmmoData.Add(CurrentWeaponInstance->GetMagazineTag(), CurrentMagazineInfo);
		}
	}
}

int32 URFEquipmentComponent::GetMagazineAmmoCountByTag(FGameplayTag MagazineTag)
{
	FEquipMagazineInfo Info = MagazineAmmoData.FindRef(MagazineTag);
	return Info.GetCurrentMagazineAmmo();
}

int32 URFEquipmentComponent::GetCurrentMagazineCapacity() const
{
	return CurrentMagazineInfo.GetCurrentMagazineCapacity();
}

bool URFEquipmentComponent::IsBulletLoaded() const
{
	if (CurrentWeaponInstance)
	{
		return CurrentWeaponInstance->IsBulletLoaded();
	}

	return false;
}

void URFEquipmentComponent::RemoveMagazineAmmoCountByTag(FGameplayTag MagazineTag, int32 StackCount)
{
	if (MagazineAmmoData.Contains(MagazineTag))
	{
		CurrentMagazineInfo.RemoveCurrentMagazineAmmo(StackCount);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CurrentMagazineInfo, this);

		MagazineAmmoData[MagazineTag].RemoveCurrentMagazineAmmo(StackCount);

		// Change the magazine mesh when ammunition is empty
		if (CurrentWeaponInstance && CurrentMagazineInfo.GetCurrentMagazineAmmo() <= 0)
		{
			CurrentWeaponInstance->ReplaceToEmptyAmmo();
		}
	}
}

void URFEquipmentComponent::ReloadNextMagazine()
{
	if (CurrentMagazineInfo.GetCurrentMagazineAmmo() > 0)
	{
		CurrentMagazineInfo.MoveNextMagazineIndex();
	}
	else
	{
		CurrentMagazineInfo.RemoveCurrentMagazine();
	}

	FGameplayTag MagazineTag = CurrentWeaponInstance->GetMagazineTag();
	if (MagazineTag.IsValid())
	{
		MagazineAmmoData[MagazineTag] = CurrentMagazineInfo;
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, CurrentMagazineInfo, this);
}

void URFEquipmentComponent::LoadBullet()
{
	if (CurrentWeaponInstance)
	{
		CurrentWeaponInstance->LoadBullet();
	}
}

void URFEquipmentComponent::UnLoadBullet()
{
	if (CurrentWeaponInstance)
	{
		CurrentWeaponInstance->UnLoadBullet();
	}
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

void URFEquipmentComponent::OnRep_CurrentMagazineInfo()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);

	FGameplayTag MagazineTag = CurrentWeaponInstance ? CurrentWeaponInstance->GetMagazineTag() : FGameplayTag();
	if (MagazineAmmoData.Contains(MagazineTag))
	{
		MagazineAmmoData[MagazineTag].CurrentMagazineIndex = CurrentMagazineInfo.CurrentMagazineIndex;
		MagazineAmmoData[MagazineTag].EquipMagazineAmmo = CurrentMagazineInfo.EquipMagazineAmmo;
	}
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
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, CurrentMagazineInfo, SharedParams);
}