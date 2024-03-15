// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimInstance.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "RF_PlayMontageAndWait.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMontageWaitSimpleDelegate);

/**
 * 
 */
UCLASS()
class RT_FPS_API URF_PlayMontageAndWait : public UAbilityTask
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate	OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate	OnBlendOut;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate	OnInterrupted;

	UPROPERTY(BlueprintAssignable)
	FMontageWaitSimpleDelegate	OnCancelled;

	UFUNCTION()
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);

	UE_DEPRECATED(5.3, "Please use OnGameplayAbilityCancelled instead. This function naming implied the Montage was already interrupted (instead, we are about to interrupt it).")
		UFUNCTION()
		void OnMontageInterrupted();

	/** Callback function for when the owning Gameplay Ability is cancelled */
	UFUNCTION()
	void OnGameplayAbilityCancelled();

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "RF_PlayMontageAndWait",
	HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
		static URF_PlayMontageAndWait* CreateRFPlayMontageAndWaitProxy(UGameplayAbility* OwningAbility,
			FName TaskInstanceName, UAnimMontage* MontageToPlay, UAnimMontage* FP_MontageToPlay, float Rate = 1.f, FName StartSection = NAME_None, bool bStopWhenAbilityEnds = true, float AnimRootMotionTranslationScale = 1.f, float StartTimeSeconds = 0.f, bool bAllowInterruptAfterBlendOut = false);

	virtual void Activate() override;

	virtual void ExternalCancel() override;

	virtual FString GetDebugString() const override;

protected:

	virtual void OnDestroy(bool AbilityEnded) override;
	
	/** Checks if the ability is playing a montage and stops that montage, returns true if a montage was stopped, false if not. */
	bool StopPlayingMontage();
	bool StopPlayingFPMontage();

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle InterruptedHandle;

	UPROPERTY()
	TObjectPtr<UAnimMontage> MontageToPlay;

	UPROPERTY()
	TObjectPtr<UAnimMontage> FP_MontageToPlay;

	UPROPERTY()
	float Rate;

	UPROPERTY()
	FName StartSection;

	UPROPERTY()
	float AnimRootMotionTranslationScale;

	UPROPERTY()
	float StartTimeSeconds;

	UPROPERTY()
	bool bStopWhenAbilityEnds;

	UPROPERTY()
	bool bAllowInterruptAfterBlendOut;
};
