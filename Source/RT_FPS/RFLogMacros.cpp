// Fill out your copyright notice in the Description page of Project Settings.


#include "RFLogMacros.h"
#include "GameFramework/Actor.h"

DEFINE_LOG_CATEGORY(LogRF);

RT_FPS_API FString GetClientServerContextString(UObject* ContextObject)
{
	ENetRole Role = ROLE_None;

	if (AActor* Actor = Cast<AActor>(ContextObject))
	{
		Role = Actor->GetLocalRole();
	}
	else if (UActorComponent* Component = Cast<UActorComponent>(ContextObject))
	{
		Role = Component->GetOwnerRole();
	}

	if (Role != ROLE_None)
	{
		return (Role == ROLE_Authority) ? TEXT("Server") : TEXT("Client");
	}
	else
	{
		return TEXT("InValid");
	}

	return TEXT("[]");
}
