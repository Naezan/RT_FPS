// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecoilParameter.generated.h"

USTRUCT(BlueprintType)
struct FRecoilParameter
{
	GENERATED_USTRUCT_BODY()

public:
	FRecoilParameter() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters", meta = (UIMin = "0", ClampMin = "0"))
	float MaxRecoilSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters", meta = (UIMin = "0", ClampMin = "0"))
	float RecoilInterpSpeed = 1.f;
};