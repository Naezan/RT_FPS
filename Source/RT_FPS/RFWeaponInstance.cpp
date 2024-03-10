// Fill out your copyright notice in the Description page of Project Settings.


#include "RFWeaponInstance.h"
#include "RFCharacter.h"
#include "RFWeaponBase.h"

void URFWeaponInstance::EquipWeapon()
{
	SetWeaponAnimInstance();

	if (AttachToWeapon)
	{
		USkeletalMeshComponent* FPMesh = GetCharacter1PMesh();
		AActor* FPAttachingWeapon = GetWorld()->SpawnActorDeferred<AActor>(AttachToWeapon, FTransform::Identity, GetPawn());
		FPAttachingWeapon->FinishSpawning(FTransform::Identity, true);
		FPAttachingWeapon->SetActorRelativeTransform(FPAttachTransform);
		FPAttachingWeapon->AttachToComponent(FPMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
		
		if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPAttachingWeapon))
		{
			FPWeapon->SetFPAttribute(true);
		}

		FPEquippedWeapon = FPAttachingWeapon;

		USkeletalMeshComponent* TPMesh = GetCharacter3PMesh();
		AActor* TPAttachingWeapon = GetWorld()->SpawnActorDeferred<AActor>(AttachToWeapon, FTransform::Identity, GetPawn());

		// if you don't TPAttachingWeapon is null
		TPAttachingWeapon->FinishSpawning(FTransform::Identity, true);
		TPAttachingWeapon->SetActorRelativeTransform(TPAttachTransform);
		TPAttachingWeapon->AttachToComponent(TPMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);

		TPEquippedWeapon = TPAttachingWeapon;
	}
}

void URFWeaponInstance::UnEquipWeapon()
{
	AActor* DetachedWeapon = GetWorld()->SpawnActorDeferred<AActor>(AttachToWeapon, TPEquippedWeapon->GetActorTransform(), nullptr);
	// Activate Simulate Physics

	FPEquippedWeapon->Destroy();
	FPEquippedWeapon = nullptr;
	TPEquippedWeapon->Destroy();
	TPEquippedWeapon = nullptr;
}

void URFWeaponInstance::SetWeaponAnimInstance()
{
	USkeletalMeshComponent* FPMesh = GetCharacter1PMesh();
	if (FPMesh && FPAnimInstance != nullptr)
	{
		FPMesh->LinkAnimClassLayers(FPAnimInstance);
	}

	USkeletalMeshComponent* TPMesh = GetCharacter3PMesh();
	if (TPMesh && TPAnimInstance != nullptr)
	{
		TPMesh->LinkAnimClassLayers(TPAnimInstance);
	}
}

APawn* URFWeaponInstance::GetPawn() const
{
	return Cast<APawn>(GetOuter());
}

ACharacter* URFWeaponInstance::GetCharacter() const
{
	return Cast<ACharacter>(GetOuter());
}

USkeletalMeshComponent* URFWeaponInstance::GetCharacter3PMesh() const
{
	ACharacter* OwningCharacter = GetCharacter();
	return OwningCharacter ? OwningCharacter->GetMesh() : nullptr;
}

USkeletalMeshComponent* URFWeaponInstance::GetCharacter1PMesh() const
{
	ARFCharacter* OwningCharacter = Cast<ARFCharacter>(GetCharacter());
	return OwningCharacter ? OwningCharacter->GetMesh1P() : nullptr;
}