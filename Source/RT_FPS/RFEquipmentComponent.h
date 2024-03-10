// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RFEquipmentComponent.generated.h"

class URFWeaponInstance;

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

public:
	// CameraShake, SpreadCurve, RecoilPattern, WeaponHitDecal properties
	bool OnEquip(TSubclassOf<URFWeaponInstance> EquipWeaponInstance);
	bool OnUnEquip();

private:
	UPROPERTY()
	TObjectPtr<URFWeaponInstance> CurrentWeaponInstance;
};
