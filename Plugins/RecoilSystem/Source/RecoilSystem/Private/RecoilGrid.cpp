// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilGrid.h"

void URecoilGrid::AddPoint(const FRecoilPoint& InPoint)
{
	FRecoilPoint& InsertedPoint = RecoilPoints.Insert_GetRef(InPoint, RecoilPoints.Num());
	InsertedPoint.Index = RecoilPoints.Num();
}

void URecoilGrid::RemovePoint(int32 Index)
{
	RecoilPoints.RemoveAt(Index);
}

void URecoilGrid::RemoveAll()
{
	if (!RecoilPoints.IsEmpty())
	{
		RecoilPoints.Empty();
	}
}

FRecoilPoint& URecoilGrid::GetPoint(int32 Index)
{
	static FRecoilPoint RecoilPoint;

	if (RecoilPoints.IsValidIndex(Index))
	{
		return RecoilPoints[Index];
	}

	return RecoilPoint;
}

bool URecoilGrid::IsValidPoint(int32 Index) const
{
	return RecoilPoints.IsValidIndex(Index);
}

bool URecoilGrid::IsEmpty() const
{
	return RecoilPoints.IsEmpty();
}

int32 URecoilGrid::GetPointsNum() const
{
	return RecoilPoints.Num();
}

const TArray<FRecoilPoint>& URecoilGrid::GetPoints() const
{
	return RecoilPoints;
}
