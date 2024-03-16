// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RFGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RT_FPS_API ARFGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ARFGameMode();

	//~AGameModeBase interface
	// if you want to change default pawn class, just implement this function and return pawn class.
	//virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	
	// If need to spawn pawn with custom spawn management system, return false this function.
	//virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	// return spawn point actor.
	//virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
	// If need to delay spawning condition, use this function.
	//virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	// If fail to respawned, then this function is called
	//virtual void FailedToRestartPlayer(AController* NewPlayer) override;
	//~End of AGameModeBase interface

	UFUNCTION(BlueprintCallable)
	const URFPlayerData* GetRFPlayerData(AController* InController) const;

protected:
	virtual void InitGameState() override;
	virtual void GenericPlayerInitialization(AController* C) override;

public:
	//void CharacterDied();
	//void CharacterRespawn();
};
