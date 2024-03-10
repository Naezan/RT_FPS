// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RFWeaponInstance.generated.h"

class UCameraShakeBase;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class RT_FPS_API URFWeaponInstance : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void EquipWeapon();
	UFUNCTION(BlueprintCallable)
	void UnEquipWeapon();

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	float WeaponSpreadMultiplier;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<URecoilPattern> WeaponRecoilPattern;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData", meta = (AllowPrivateAccess = "true"))
	//TSubclassOf<AWeaponActorBase> EquipWeapon;
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponAbilityData", meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<UAbilitySet> WepaonAbilitySet;

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
