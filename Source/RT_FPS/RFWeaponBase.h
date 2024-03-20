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
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable)
	void SetFPAttribute(bool IsFP);
	UFUNCTION(BlueprintPure)
	bool GetFPAttribute() const { return IsFPWeapon; }
	UFUNCTION(BlueprintCallable)
	void SetAttachedMagActor(AActor* InMagActor);
	UFUNCTION(BlueprintPure)
	AActor* GetAttachedMagActor() const { return AttachedMagActor; }

private:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	uint8 IsFPWeapon : 1;
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AActor> AttachedMagActor;
};
