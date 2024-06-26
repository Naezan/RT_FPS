// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RecoilParameter.h"
#include "RecoilPatternAsset.generated.h"

struct FRecoilPoint;

/**
 * 
 */
UCLASS(BlueprintType)
class RECOILSYSTEM_API URecoilPatternAsset : public UObject
{
	GENERATED_BODY()
	
public:
	URecoilPatternAsset(const FObjectInitializer& ObjectInitializer);

	class URecoilGrid* GetRecoilGrid() const;

	int32 GetRecoilPointsNum() const;
	const FRecoilPoint& GetRecoilPointAt(int32 RecoilIndex) const;

public:
	UPROPERTY()
	class URecoilGrid* RecoilGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FRecoilParameter RecoilInterpParams;
};
