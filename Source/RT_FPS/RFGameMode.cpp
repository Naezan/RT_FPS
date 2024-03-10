// Fill out your copyright notice in the Description page of Project Settings.


#include "RFGameMode.h"

ARFGameMode::ARFGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/RTFPS/Blueprint/Character/B_RFCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassFinder(TEXT("/Game/RTFPS/Blueprint/Character/B_RFPlayerController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;
}