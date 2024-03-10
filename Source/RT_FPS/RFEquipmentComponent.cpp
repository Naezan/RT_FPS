// Fill out your copyright notice in the Description page of Project Settings.


#include "RFEquipmentComponent.h"
#include "RFWeaponInstance.h"

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
		UE_LOG(LogTemp, Warning, TEXT("Already has weapon."));
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
