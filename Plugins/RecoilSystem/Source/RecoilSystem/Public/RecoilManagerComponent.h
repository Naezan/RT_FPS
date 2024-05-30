// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RecoilManagerComponent.generated.h"

UCLASS(ClassGroup = (Custom), Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent) )
class RECOILSYSTEM_API URecoilManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	URecoilManagerComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void TriggerRecoil();
	UFUNCTION(BlueprintCallable)
	void CompleteRecoil();

	AController* GetOwningController() const;
	UFUNCTION(BlueprintCallable, Category = "Recoil")
	FRotator GetControlRotation() const;

	UFUNCTION(Reliable, Client, BlueprintCallable, Category = "Recoil")
	void SetControlRotation(const FRotator& Rotation);

	UFUNCTION(BlueprintPure, Category = "Recoil")
	class URecoilPatternAsset* GetRecoilPattern() const;

	UFUNCTION(BLueprintPure, Category = "Recoil")
	FRotator GetRecoilDeltaRotation(int32 InRecoilIndex) const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Replicated)
	FRotator RepliactedRecoilRotation;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Recoil", meta = (AllowPrivateAccess="true"))
	class URecoilPatternAsset* RecoilPatternAsset;

	UPROPERTY(BlueprintReadOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
	float RecoilSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Recoil", meta = (AllowPrivateAccess = "true"))
	int32 RecoilIndex = 0;
};
