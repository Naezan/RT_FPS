// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "RFGlobalStruct.h"
#include "RFWeaponInstance.generated.h"

class UCameraShakeBase;
class URFAbilitySet;
class ARFWeaponBase;
class ARFMagazineBase;
struct FGameplayTag;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RT_FPS_API URFWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	URFWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION()
	void InitializeWeaponAnimInstance();
	UFUNCTION(BlueprintCallable)
	void EquipWeapon();
	UFUNCTION(BlueprintCallable)
	void UnEquipWeapon();

	UFUNCTION()
	void SpawnWeapon();
	UFUNCTION()
	void SpawnWeaponMagazine();

	// FAttachmentTransformRules is not support UFUNCTION Macro
	AActor* SpawnActorByMesh(TSubclassOf<AActor> ActorClass, AActor* Owner, USkeletalMeshComponent* AttachMesh, const FAttachmentTransformRules& AttachmentRules, FName AttachSocket = NAME_None);

	UFUNCTION(BlueprintCallable)
	void GiveWeaponAbility();
	UFUNCTION(BlueprintCallable)
	void RemoveWeaponAbility();

public:
	UFUNCTION(BlueprintPure)
	APawn* GetPawn() const;
	UFUNCTION(BlueprintPure)
	ACharacter* GetCharacter() const;
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetCharacterTPMesh() const;
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetCharacterFPMesh() const;
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetCharacterFPLegMesh() const;
	UFUNCTION(BlueprintPure)
	USkeletalMeshComponent* GetWeaponMesh(AActor* WeaponMesh) const;
	UFUNCTION(BlueprintPure)
	AActor* GetFPWeaponActor() const { return FPEquippedWeapon; }
	UFUNCTION(BlueprintPure)
	AActor* GetTPWeaponActor() const { return TPEquippedWeapon; }

	// Contains information on how much the player's aim will rotate at the Yaw angle(left/right).
	FORCEINLINE float GetSpreadAngle() const { return FireSpreadAngle; }
	FORCEINLINE float GetSpreadHalfAngle() const { return FireSpreadAngle / 2.f; }
	FORCEINLINE float GetSpreadAngleMultiplier() const { return bIsFirstShotAccuracy ? 0.0f : FireSpreadAngleMultiplier; }
	// How many bullet using when firing one shot.
	FORCEINLINE int32 GetOneShotCartridge() const { return FireCartridge; }
	// How bullet gathered together, 0.1 is spread worldwide(not recommended), 1.0 is default cluster, 1.0 > is more clustered on center of aim.
	FORCEINLINE float GetFireCluster() const { return FireSpreadCluster; }
	// Weapon Damage range when firing, same as trace thickness
	FORCEINLINE float GetFireTraceRadius() const { return BulletRadius; }
	// Max weapon firing range
	FORCEINLINE float GetFireEffectiveRange() const { return WeaponEffectiveRange; }
	FORCEINLINE bool IsFirstShotAccuracy() const { return bIsFirstShotAccuracy; }
	const FVector GetMuzzleLocation() const;

	//~Magazine
	const TArray<FMagazineInfo>& GetMagazineInfos() const { return MagazineInfos; }
	int32 GetMagazineCapacity() const { return MagazineInfos.Num(); }
	FGameplayTag GetMagazineTag() const { return MagazineTag; }
	void ReplaceToEmptyAmmo();
	// ~Magazine

	bool IsBulletLoaded();
	void LoadBullet();
	void UnLoadBullet();

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> WeaponCameraShake;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ForceUnits = deg))
	float FireSpreadAngle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	float FireSpreadAngleMultiplier = 1.f;
	// How many bullet using when firing one shot.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ForceUnits = x))
	float FireCartridge = 1.f;
	// How bullet gathered together, 0.1 is spread worldwide(not recommended), 1.0 is default cluster, 1.0 > is more clustered on center of aim.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ClampMin = 0.1, ForceUnits = x))
	float FireSpreadCluster = 1.f;
	// Weapon Damage range when firing, same as trace thickness
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ForceUnits = cm))
	float BulletRadius = 3.f;
	// Max weapon firing range
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true", ForceUnits = cm))
	float WeaponEffectiveRange = 50000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	uint8 bIsFirstShotAccuracy : 1;

	// TODO: Apply weapon recoil pattern.
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<URecoilPattern> WeaponRecoilPattern;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponAbilityData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URFAbilitySet> WepaonAbilitySet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARFWeaponBase> WeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData", meta = (AllowPrivateAccess = "true"))
	FName WeaponAttachSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData", meta = (AllowPrivateAccess = "true"))
	FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData|Mag", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ARFMagazineBase> WeaponMagazineClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData|Mag", meta = (AllowPrivateAccess = "true"))
	FName MagAttachSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData|Mag", meta = (AllowPrivateAccess = "true"))
	TArray<FMagazineInfo> MagazineInfos;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData|Mag", meta = (AllowPrivateAccess = "true"))
	FGameplayTag MagazineTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnEquipData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> UnEquipWeaponClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnEquipData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> UnEquipWeaponWithMagClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UnEquipData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> UnEquipWeaponMagClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> FPUpperAnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> TPUpperAnimInstance;

private:
	UFUNCTION()
	void OnRep_FPEquippedWeapon();
	UFUNCTION()
	void OnRep_TPEquippedWeapon();
	UFUNCTION()
	void OnRep_FPWeaponMag();
	UFUNCTION()
	void OnRep_TPWeaponMag();

private:
	UPROPERTY(ReplicatedUsing = OnRep_FPEquippedWeapon, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> FPEquippedWeapon;
	UPROPERTY(ReplicatedUsing = OnRep_TPEquippedWeapon, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TPEquippedWeapon;
	UPROPERTY(ReplicatedUsing = OnRep_FPWeaponMag, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> FPWeaponMag;
	UPROPERTY(ReplicatedUsing = OnRep_TPWeaponMag, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> TPWeaponMag;
};
