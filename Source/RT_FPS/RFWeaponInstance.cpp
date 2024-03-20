// Fill out your copyright notice in the Description page of Project Settings.


#include "RFWeaponInstance.h"
#include "GameFramework/Character.h"
#include "RFWeaponBase.h"
#include "RFMagazineBase.h"
#include "AbilitySystem/RFAbilitySet.h"
#include "Interface/RFMeshInterface.h"
#include "AbilitySystemInterface.h"
#include "RFLogMacros.h"
#include "Net/UnrealNetwork.h"

URFWeaponInstance::URFWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bIsFirstShotAccuracy = true;
}

void URFWeaponInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, FPEquippedWeapon);
	DOREPLIFETIME(ThisClass, TPEquippedWeapon);
	DOREPLIFETIME(ThisClass, FPEquippedWeaponMag);
	DOREPLIFETIME(ThisClass, TPEquippedWeaponMag);
}

void URFWeaponInstance::InitializeWeaponAnimInstance()
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

	USkeletalMeshComponent* LegMesh = GetCharacterFPLegMesh();
	if (LegMesh && TPAnimInstance != nullptr)
	{
		LegMesh->LinkAnimClassLayers(TPAnimInstance);
	}
}

void URFWeaponInstance::EquipWeapon()
{
	//Server Only
	InitializeWeaponAnimInstance();

	if (GetPawn() && GetPawn()->HasAuthority())
	{
		SpawnWeapon();
		SpawnWeaponMagazine();

		GiveWeaponAbility();
	}
}

void URFWeaponInstance::UnEquipWeapon()
{
	RemoveWeaponAbility();

	if (WeaponClass && GetPawn() && GetPawn()->HasAuthority())
	{
		AActor* DetachedWeapon = GetWorld()->SpawnActorDeferred<AActor>(WeaponClass, TPEquippedWeapon->GetActorTransform(), nullptr);
		// Activate Simulate Physics
	}

	FPEquippedWeapon->Destroy();
	TPEquippedWeapon->Destroy();
	FPEquippedWeapon = nullptr;
	TPEquippedWeapon = nullptr;
	FPEquippedWeaponMag = nullptr;
	TPEquippedWeaponMag = nullptr;
}

void URFWeaponInstance::SpawnWeapon()
{
	FPEquippedWeapon = SpawnActorByMesh(
	WeaponClass, 
	GetPawn(), 
	GetCharacterFPMesh(), 
	FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
	WeaponAttachSocketName);
	if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPEquippedWeapon))
	{
		FPWeapon->SetFPAttribute(true);
	}

	TPEquippedWeapon = SpawnActorByMesh(
	WeaponClass, 
	GetPawn(), 
	GetCharacterTPMesh(), 
	FAttachmentTransformRules::SnapToTargetNotIncludingScale, 
	WeaponAttachSocketName);
}

void URFWeaponInstance::SpawnWeaponMagazine()
{
	FPEquippedWeaponMag = SpawnActorByMesh(
	WeaponMagazineClass, 
	FPEquippedWeapon, 
	GetWeaponMesh(FPEquippedWeapon), 
	FAttachmentTransformRules::KeepRelativeTransform, 
	MagAttachSocketName);
	if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPEquippedWeapon))
	{
		FPWeapon->SetAttachedMagActor(FPEquippedWeaponMag);
	}

	TPEquippedWeaponMag = SpawnActorByMesh(
	WeaponMagazineClass, 
	TPEquippedWeapon, 
	GetWeaponMesh(TPEquippedWeapon), 
	FAttachmentTransformRules::KeepRelativeTransform, 
	MagAttachSocketName);
	if (ARFWeaponBase* TPWeapon = Cast<ARFWeaponBase>(TPEquippedWeapon))
	{
		TPWeapon->SetAttachedMagActor(TPEquippedWeaponMag);
	}
}

AActor* URFWeaponInstance::SpawnActorByMesh(TSubclassOf<AActor> ActorClass, AActor* Owner, USkeletalMeshComponent* AttachMesh, const FAttachmentTransformRules& AttachmentRules, FName AttachSocket)
{
	AActor* AttachToActor = (ActorClass != nullptr) ? GetWorld()->SpawnActorDeferred<AActor>(ActorClass, FTransform::Identity, Owner) : nullptr;

	// if you don't AttachToActor is null
	AttachToActor->FinishSpawning(FTransform::Identity, true);
	AttachToActor->SetActorRelativeTransform(FTransform::Identity);

	if (AttachMesh)
	{
		AttachToActor->AttachToComponent(AttachMesh, AttachmentRules, AttachSocket);
	}

	return AttachToActor;
}

void URFWeaponInstance::GiveWeaponAbility()
{
	if (ACharacter* OwningPawn = Cast<ACharacter>(GetOuter()))
	{
		if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OwningPawn))
		{
			UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
			if (OwningPawn->GetLocalRole() != ROLE_Authority || ASC == nullptr)
			{
				UE_LOG(LogRF, Warning, TEXT("Client Cant add ability(OwningPawn Role:[%d]) or AbilitySystemComponent Validation(IsValid:[%d]), in funciton([%s])"), OwningPawn->GetLocalRole(), ASC != nullptr, *RF_CUR_CLASS_FUNC);
				return;
			}
			WepaonAbilitySet->GiveAbilities(ASC, this);
		}
	}
}

void URFWeaponInstance::RemoveWeaponAbility()
{
	if (ACharacter* OwningPawn = Cast<ACharacter>(GetOuter()))
	{
		if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OwningPawn))
		{
			UAbilitySystemComponent* ASC = ASCInterface->GetAbilitySystemComponent();
			if (ASC != nullptr)
			{
				WepaonAbilitySet->RemoveAbilities(ASC);
			}
		}
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

void URFWeaponInstance::OnRep_FPEquippedWeapon()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

void URFWeaponInstance::OnRep_TPEquippedWeapon()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

void URFWeaponInstance::OnRep_FPEquippedWeaponMag()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

void URFWeaponInstance::OnRep_TPEquippedWeaponMag()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
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
	IRFMeshInterface* OwningCharacter = Cast<IRFMeshInterface>(GetCharacter());
	return OwningCharacter ? OwningCharacter->GetFPMesh() : nullptr;
}

USkeletalMeshComponent* URFWeaponInstance::GetCharacterFPLegMesh() const
{
	IRFMeshInterface* OwningCharacter = Cast<IRFMeshInterface>(GetCharacter());
	return OwningCharacter ? OwningCharacter->GetFPLegMesh() : nullptr;
}

USkeletalMeshComponent* URFWeaponInstance::GetWeaponMesh(AActor* WeaponMesh) const
{
	return WeaponMesh ? Cast<USkeletalMeshComponent>(WeaponMesh->GetRootComponent()) : nullptr;
}
