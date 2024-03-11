// Fill out your copyright notice in the Description page of Project Settings.


#include "RFEquipmentComponent.h"
#include "RFWeaponInstance.h"
#include "RFLogMacros.h"

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
	if (CurrentWeaponInstance)
	{
		UE_LOG(LogRF, Warning, TEXT("Already has weapon."));
		return false;
	}

	if (EquipWeaponInstance == nullptr)
	{
		UE_LOG(LogRF, Warning, TEXT("EquipWeapon Instance is invalid on [%s]"), *GetNameSafe(this));
		return false;
	}

	CurrentWeaponInstance = NewObject<URFWeaponInstance>(GetOwner(), EquipWeaponInstance);
	CurrentWeaponInstance->EquipWeapon();

	return true;
}

bool URFEquipmentComponent::OnUnEquip()
{
	if (!CurrentWeaponInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Doesn't have equipped weapon."));
		return false;
	}

	CurrentWeaponInstance->UnEquipWeapon();
	CurrentWeaponInstance = nullptr;

	return true;
}
