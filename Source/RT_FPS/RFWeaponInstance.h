// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RFWeaponInstance.generated.h"

class UCameraShakeBase;
class URFAbilitySet;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RT_FPS_API URFWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	URFWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	UFUNCTION(BlueprintCallable)
	void EquipWeapon();
	UFUNCTION(BlueprintCallable)
	void UnEquipWeapon();

	UFUNCTION(BlueprintCallable)
	void GiveWeaponAbility();
	UFUNCTION(BlueprintCallable)
	void RemoveWeaponAbility();

public:
	FORCEINLINE float GetSpreadAngle() const { return FireSpreadAngle; }
	FORCEINLINE float GetSpreadAngleMultiplier() const { return bIsFirstShotAccuracy ? 0.0f : FireSpreadAngleMultiplier; }
	FORCEINLINE int32 GetOneShotCartridge() const { return FireCartridge; }
	FORCEINLINE float GetFireCluster() const { return FireSpreadCluster; }
	FORCEINLINE float GetFireTraceRadius() const { return BulletRadius; }
	FORCEINLINE float GetFireEffectiveRange() const { return WeaponEffectiveRange; }
	FORCEINLINE bool IsFirstShotAccuracy() const { return bIsFirstShotAccuracy; }
	
	const FVector GetMuzzleLocation() const;

protected:
	void SetWeaponAnimInstance();

private:
	UFUNCTION()
	APawn* GetPawn() const;
	UFUNCTION()
	ACharacter* GetCharacter() const;
	UFUNCTION()
	USkeletalMeshComponent* GetCharacter3PMesh() const;
	UFUNCTION()
	USkeletalMeshComponent* GetCharacter1PMesh() const;

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
	FName AttachSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData", meta = (AllowPrivateAccess = "true"))
	FTransform FPAttachTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData", meta = (AllowPrivateAccess = "true"))
	FTransform TPAttachTransform;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EquipData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> AttachToWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> FPAnimInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AnimData", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimInstance> TPAnimInstance;

private:
	UPROPERTY()
	TObjectPtr<AActor> FPEquippedWeapon;
	UPROPERTY()
	TObjectPtr<AActor> TPEquippedWeapon;
};
