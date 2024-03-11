// Fill out your copyright notice in the Description page of Project Settings.


#include "RFGameMode.h"
#include "RFPlayerState.h"
#include "RFPlayerData.h"
#include "RFLogMacros.h"

ARFGameMode::ARFGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/RTFPS/Blueprint/Character/B_RFCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/RTFPS/Blueprint/Character/B_RFPlayerController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
}

APawn* ARFGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	APawn* SpawnedPawn = Super::SpawnDefaultPawnAtTransform_Implementation(NewPlayer, SpawnTransform);
	
	// initialize additional playerdata on this block
	
	return SpawnedPawn;
}

bool ARFGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return Super::PlayerCanRestart_Implementation(Player);
}

const URFPlayerData* ARFGameMode::GetRFPlayerData(AController* InController) const
{
	if (InController != nullptr)
	{
		if (ARFPlayerState* ControllerPS = InController->GetPlayerState<ARFPlayerState>())
		{
			return ControllerPS->GetPlayerData();
		}
	}

	return nullptr;
}
