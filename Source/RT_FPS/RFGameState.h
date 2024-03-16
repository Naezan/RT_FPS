// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Interface/GameStateGlobalDelegateInterface.h"
#include "RFGameState.generated.h"


/**
 * 
 */
UCLASS()
class RT_FPS_API ARFGameState : public AGameStateBase, public IGameStateGlobalDelegateInterface
{
	GENERATED_BODY()

public:
	virtual FPlayerInitialized& GetPlayerInitializedDelegate() override;
	
private:
	FPlayerInitialized OnPlayerInitialized;
};
