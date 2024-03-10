// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFWeaponBase.generated.h"

UCLASS()
class RT_FPS_API ARFWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARFWeaponBase();

public:
	UFUNCTION(BlueprintCallable)
	void SetFPAttribute(bool IsFP);
	UFUNCTION(BlueprintPure)
	bool GetFPAttribute() const { return IsFPWeapon; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 IsFPWeapon : 1;
};
