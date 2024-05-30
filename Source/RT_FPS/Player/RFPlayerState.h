// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "RFPlayerState.generated.h"

class URFPlayerData;
class URFAbilitySystemComponent;
class URFAttributeSet;

/**
 *
 */
UCLASS()
class RT_FPS_API ARFPlayerState : public APlayerState, public IAbilitySystemInterface, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ARFPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostInitializeComponents() override;

	UFUNCTION(BlueprintCallable, Category = "AbilitySystemComponent")
	URFAbilitySystemComponent* GetRFAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	URFAttributeSet* GetAttributeSet() const;

	//~ILyraTeamAgentInterface interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~End of ILyraTeamAgentInterface interface

public:
	const URFPlayerData* GetPlayerData() const { return PlayerData; }
	void SetPlayerData(const URFPlayerData* InPawnData);
	void GiveDefaultAbilities();

private:
	UFUNCTION()
	void OnRep_PlayerTeamID(FGenericTeamId OldTeamID);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Replicated)
	TObjectPtr<const URFPlayerData> PlayerData;

private:
	// Ability system component that player has.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystemComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URFAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystemComponent", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<URFAttributeSet> HealthAttributeSet;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerTeamID)
	FGenericTeamId PlayerTeamID;
};
