// Fill out your copyright notice in the Description page of Project Settings.


#include "RFWeaponInstance.h"
#include "RFCharacter.h"
#include "RFWeaponBase.h"
#include "AbilitySystem/RFAbilitySet.h"
#include "AbilitySystem/RFAbilitySystemComponent.h"
#include "RFLogMacros.h"

URFWeaponInstance::URFWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFirstShotAccuracy = true;
}

void URFWeaponInstance::EquipWeapon()
{
	SetWeaponAnimInstance();

	if (AttachToWeapon)
	{
		USkeletalMeshComponent* FPMesh = GetCharacterFPMesh();
		AActor* FPAttachingWeapon = GetWorld()->SpawnActorDeferred<AActor>(AttachToWeapon, FTransform::Identity, GetPawn());
		FPAttachingWeapon->FinishSpawning(FTransform::Identity, true);
		FPAttachingWeapon->SetActorRelativeTransform(FPAttachTransform);
		FPAttachingWeapon->AttachToComponent(FPMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);
		
		if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPAttachingWeapon))
		{
			FPWeapon->SetFPAttribute(true);
		}

		FPEquippedWeapon = FPAttachingWeapon;

		USkeletalMeshComponent* TPMesh = GetCharacterTPMesh();
		AActor* TPAttachingWeapon = GetWorld()->SpawnActorDeferred<AActor>(AttachToWeapon, FTransform::Identity, GetPawn());

		// if you don't TPAttachingWeapon is null
		TPAttachingWeapon->FinishSpawning(FTransform::Identity, true);
		TPAttachingWeapon->SetActorRelativeTransform(TPAttachTransform);
		TPAttachingWeapon->AttachToComponent(TPMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName);

		TPEquippedWeapon = TPAttachingWeapon;

		GiveWeaponAbility();
	}
}

void URFWeaponInstance::UnEquipWeapon()
{
	RemoveWeaponAbility();

	AActor* DetachedWeapon = GetWorld()->SpawnActorDeferred<AActor>(AttachToWeapon, TPEquippedWeapon->GetActorTransform(), nullptr);
	// Activate Simulate Physics

	FPEquippedWeapon->Destroy();
	FPEquippedWeapon = nullptr;
	TPEquippedWeapon->Destroy();
	TPEquippedWeapon = nullptr;
}

void URFWeaponInstance::GiveWeaponAbility()
{
	if (ARFCharacter* OwningPawn = Cast<ARFCharacter>(GetOuter()))
	{
		URFAbilitySystemComponent* ASC = OwningPawn->GetCachedAbilitySystemComponent();

		if (OwningPawn->GetLocalRole() != ROLE_Authority || ASC == nullptr)
		{
			UE_LOG(LogRF, Warning, TEXT("Client Cant add ability -> [%d] or AbilitySystemComponent not valid -> [%d] : [%s]"), OwningPawn->GetLocalRole(), IsValid(ASC), *RF_CUR_CLASS_FUNC);
			return;
		}
		WepaonAbilitySet->GiveAbilities(ASC, this);
	}
}

void URFWeaponInstance::RemoveWeaponAbility()
{
	if (ARFCharacter* OwningPawn = Cast<ARFCharacter>(GetOuter()))
	{
		URFAbilitySystemComponent* ASC = OwningPawn->GetCachedAbilitySystemComponent();
		WepaonAbilitySet->RemoveAbilities(ASC);
	}
}

const FVector URFWeaponInstance::GetMuzzleLocation() const
{
	if (USkeletalMeshComponent* WeaponMesh = Cast<USkeletalMeshComponent>(FPEquippedWeapon->GetRootComponent()))
	{
		return WeaponMesh->GetSocketLocation(FName("Muzzle"));
	}

	return FVector();
}

void URFWeaponInstance::SetWeaponAnimInstance()
{
	USkeletalMeshComponent* FPMesh = GetCharacterFPMesh();
	if (FPMesh && FPAnimInstance != nullptr)
	{
		FPMesh->LinkAnimClassLayers(FPAnimInstance);
	}

	USkeletalMeshComponent* TPMesh = GetCharacterTPMesh();
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

USkeletalMeshComponent* URFWeaponInstance::GetCharacterTPMesh() const
{
	ACharacter* OwningCharacter = GetCharacter();
	return OwningCharacter ? OwningCharacter->GetMesh() : nullptr;
}

USkeletalMeshComponent* URFWeaponInstance::GetCharacterFPMesh() const
{
	ARFCharacter* OwningCharacter = Cast<ARFCharacter>(GetCharacter());
	return OwningCharacter ? OwningCharacter->GetFPMesh() : nullptr;
}