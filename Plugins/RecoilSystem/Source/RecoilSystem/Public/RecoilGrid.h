// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RecoilGrid.generated.h"

USTRUCT(BlueprintType)
struct RECOILSYSTEM_API FRecoilPoint
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point")
	FVector2D PointCoord = FVector2D::ZeroVector;

	// Recoil Pattern index starting from 1
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Point")
	int32 Index;

	static constexpr int32 PointSize = 24.f;
};

/**
 * 
 */
UCLASS()
class RECOILSYSTEM_API URecoilGrid : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddPoint(const FRecoilPoint& InPoint);

	UFUNCTION(BlueprintCallable)
	void RemovePoint(int32 Index);

	UFUNCTION(BlueprintCallable)
	void RemoveAll();

	UFUNCTION(BlueprintCallable)
	FRecoilPoint& GetPoint(int32 Index);

	UFUNCTION(BlueprintCallable)
	bool IsValidPoint(int32 Index) const;

	UFUNCTION(BlueprintCallable)
	bool IsEmpty() const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetPointsNum() const;

	UFUNCTION(BlueprintCallable)
	const TArray<FRecoilPoint>& GetPoints() const;
	
protected:
	UPROPERTY()
	TArray<FRecoilPoint> RecoilPoints;
};
