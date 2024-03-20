// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RFMagazineBase.generated.h"

UCLASS()
class RT_FPS_API ARFMagazineBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ARFMagazineBase();

protected:
	virtual void BeginPlay() override;
};
