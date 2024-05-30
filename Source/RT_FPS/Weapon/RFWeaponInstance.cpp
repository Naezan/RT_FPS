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
	DOREPLIFETIME(ThisClass, FPWeaponMag);
	DOREPLIFETIME(ThisClass, TPWeaponMag);
}

void URFWeaponInstance::InitializeWeaponAnimInstance()
{
	if(FPUpperAnimInstance != nullptr)
	{
		if (USkeletalMeshComponent* FPMesh = GetCharacterFPMesh())
		{
			FPMesh->LinkAnimClassLayers(FPUpperAnimInstance);
		}
	}

	if (TPUpperAnimInstance != nullptr)
	{
		if (USkeletalMeshComponent* TPMesh = GetCharacterTPMesh())
		{
			TPMesh->LinkAnimClassLayers(TPUpperAnimInstance);
		}
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

	if (GetPawn() && GetPawn()->HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		// If have a mag, spawn combination with mag.
		if (TPWeaponMag->GetAttachParentActor() == TPEquippedWeapon && UnEquipWeaponWithMagClass)
		{
			GetWorld()->SpawnActor<AActor>(UnEquipWeaponWithMagClass, TPWeaponMag->GetActorTransform(), SpawnParams);
		}
		// If not have a mag, spawn separately.
		else if(UnEquipWeaponClass && UnEquipWeaponMagClass)
		{
			GetWorld()->SpawnActor<AActor>(UnEquipWeaponClass, TPEquippedWeapon->GetActorTransform(), SpawnParams);
			GetWorld()->SpawnActor<AActor>(UnEquipWeaponMagClass, TPEquippedWeapon->GetActorTransform(), SpawnParams);
		}
	}

	FPEquippedWeapon->Destroy();
	TPEquippedWeapon->Destroy();
	FPEquippedWeapon = nullptr;
	TPEquippedWeapon = nullptr;
	FPWeaponMag->Destroy();
	TPWeaponMag->Destroy();
	FPWeaponMag = nullptr;
	TPWeaponMag = nullptr;
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
		TPWeaponAttachSocketName);
}

void URFWeaponInstance::SpawnWeaponMagazine()
{
	FPWeaponMag = SpawnActorByMesh(
		WeaponMagazineClass,
		FPEquippedWeapon,
		GetWeaponMesh(FPEquippedWeapon),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		MagAttachSocketName);
	FPWeaponMag->SetActorRelativeTransform(FTransform::Identity);
	if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPEquippedWeapon))
	{
		FPWeapon->SetAttachedMagActor(FPWeaponMag);
	}

	TPWeaponMag = SpawnActorByMesh(
		WeaponMagazineClass,
		TPEquippedWeapon,
		GetWeaponMesh(TPEquippedWeapon),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		MagAttachSocketName);
	TPWeaponMag->SetActorRelativeTransform(FTransform::Identity);
	if (ARFWeaponBase* TPWeapon = Cast<ARFWeaponBase>(TPEquippedWeapon))
	{
		TPWeapon->SetAttachedMagActor(TPWeaponMag);
	}
}

AActor* URFWeaponInstance::SpawnActorByMesh(TSubclassOf<AActor> ActorClass, AActor* Owner, USkeletalMeshComponent* AttachMesh, const FAttachmentTransformRules& AttachmentRules, FName AttachSocket)
{
	AActor* AttachToActor = (ActorClass != nullptr) ? GetWorld()->SpawnActorDeferred<AActor>(ActorClass, FTransform::Identity, Owner) : nullptr;

	// if you don't AttachToActor is null
	AttachToActor->FinishSpawning(FTransform::Identity, true);

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
		return WeaponMesh->GetSocketLocation(MuzzleSocketName);
	}

	return FVector();
}

void URFWeaponInstance::ReplaceToEmptyAmmo()
{
	if (IMagazineInterface* MagazinInterface = Cast<IMagazineInterface>(FPWeaponMag))
	{
		MagazinInterface->OnAllAmmoConsumed();
	}
	if (IMagazineInterface* MagazinInterface = Cast<IMagazineInterface>(TPWeaponMag))
	{
		MagazinInterface->OnAllAmmoConsumed();
	}
}

bool URFWeaponInstance::IsBulletLoaded()
{
	if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPEquippedWeapon))
	{
		return FPWeapon->IsBulletLoaded();
	}

	return false;
}

void URFWeaponInstance::LoadBullet()
{
	if(ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPEquippedWeapon))
	{
		FPWeapon->BulletLoad();
	}
	if (ARFWeaponBase* TPWeapon = Cast<ARFWeaponBase>(TPEquippedWeapon))
	{
		TPWeapon->BulletLoad();
	}
}

void URFWeaponInstance::UnLoadBullet()
{
	if (ARFWeaponBase* FPWeapon = Cast<ARFWeaponBase>(FPEquippedWeapon))
	{
		FPWeapon->BulletUnLoad();
	}
	if (ARFWeaponBase* TPWeapon = Cast<ARFWeaponBase>(TPEquippedWeapon))
	{
		TPWeapon->BulletUnLoad();
	}
}

void URFWeaponInstance::OnRep_FPEquippedWeapon()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

void URFWeaponInstance::OnRep_TPEquippedWeapon()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

void URFWeaponInstance::OnRep_FPWeaponMag()
{
	UE_LOG(LogRF, Warning, TEXT("[%s]"), *RF_CUR_CLASS_FUNC);
}

void URFWeaponInstance::OnRep_TPWeaponMag()
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
