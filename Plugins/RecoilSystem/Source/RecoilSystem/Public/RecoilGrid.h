// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Guid.h"
#include "RecoilGrid.generated.h"

USTRUCT(BlueprintType)
struct RECOILSYSTEM_API FRecoilPoint
{
	GENERATED_BODY()

public:
	FRecoilPoint();
	FRecoilPoint(FVector2D InPoint) : PointCoord(InPoint)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Point")
	FVector2D PointCoord;

	// Recoil Pattern hash
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Point")
	FGuid Hash;

	static constexpr int32 PointSize = 12.f;
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
	FGuid AddPoint(const FRecoilPoint& InPoint);

	UFUNCTION(BlueprintCallable)
	void RemovePoint(int32 Index);
	UFUNCTION(BlueprintCallable)
	void RemovePointByKey(FGuid Key);

	UFUNCTION(BlueprintCallable)
	void RemoveAll();

	UFUNCTION(BlueprintCallable)
	FRecoilPoint& GetPoint(int32 Index);
	UFUNCTION(BlueprintCallable)
	FRecoilPoint& GetPointByKey(FGuid Key);

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
