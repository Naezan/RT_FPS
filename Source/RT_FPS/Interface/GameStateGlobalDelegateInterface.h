// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameStateGlobalDelegateInterface.generated.h"

/**
 * Post login event on GameMode, triggered when a player or bot joins the game
 *
 * This is called after the player has finished initialization
 */
DECLARE_MULTICAST_DELEGATE(FPlayerInitialized);

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGameStateGlobalDelegateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RT_FPS_API IGameStateGlobalDelegateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FPlayerInitialized& GetPlayerInitializedDelegate() = 0;
};
