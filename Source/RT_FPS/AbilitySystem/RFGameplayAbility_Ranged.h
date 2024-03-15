// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RFGameplayAbility.h"
#include "RFGameplayAbility_Ranged.generated.h"

class URFWeaponInstance;

/**
 * 
 */
UCLASS()
class RT_FPS_API URFGameplayAbility_Ranged : public URFGameplayAbility
{
	GENERATED_BODY()

protected:
	//~UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	//~End of UGameplayAbility interface
	
public:
	// Client Only. Found target use line trace and sweep trace then send targetdata to server.
	UFUNCTION(BlueprintCallable)
	void ClientTargetTrace();

	URFWeaponInstance* GetWeaponInstance() const;

private:
	void PerformBulletTrace(OUT TArray<FHitResult>& OutHit);
	void BulletTraceByCartridge(OUT TArray<FHitResult>& OutHit, const FVector& TraceStart, const FVector& TraceEnd, int32 InCartridgeCount, float SweepRadius);
	FHitResult BulletTrace(OUT TArray<FHitResult>& OutHit, const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius);
	bool IsHitPlayer(const FHitResult& InHitResult);

	void CalculateCameraTransform(OUT FVector& OutLocation, OUT FRotator& OutRotation);
	const FVector GetCameraLocation();
	const FRotator GetCameraRotation();
	const FVector ApplyRecoilToBulletDirection(const FVector& InDirection, float SpreadHalfAngle, float Cohesiveness);
};
