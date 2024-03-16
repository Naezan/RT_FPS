// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RFEquipmentComponent.generated.h"

class URFWeaponInstance;
class UAnimInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RT_FPS_API URFEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URFEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

public:
	// CameraShake, SpreadCurve, RecoilPattern, WeaponHitDecal properties
	bool OnEquip(TSubclassOf<URFWeaponInstance> EquipWeaponInstance);
	bool OnUnEquip();

	UFUNCTION(BlueprintPure)
	AActor* GetReplicatedFPWeapon() const;
	UFUNCTION(BlueprintPure)
	AActor* GetReplicatedTPWeapon() const;

public:
	UFUNCTION()
	void OnRep_CurrentWeaponInstance();

private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeaponInstance, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URFWeaponInstance> CurrentWeaponInstance;
};
