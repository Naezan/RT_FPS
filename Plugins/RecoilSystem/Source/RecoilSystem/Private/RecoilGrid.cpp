// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilGrid.h"

FGuid URecoilGrid::AddPoint(const FRecoilPoint& InPoint)
{
	FRecoilPoint& InsertedPoint = RecoilPoints.Insert_GetRef(InPoint, RecoilPoints.Num());
	InsertedPoint.Hash = FGuid::NewGuid();

	return InsertedPoint.Hash;
}

void URecoilGrid::RemovePoint(int32 Index)
{
	RecoilPoints.RemoveAt(Index);
}

void URecoilGrid::RemovePointByKey(FGuid Key)
{
	for (int32 i = 0; i < RecoilPoints.Num(); i++)
	{
		if (RecoilPoints[i].Hash == Key)
		{
			RecoilPoints.RemoveAt(i);
			break;
		}
	}
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

FRecoilPoint& URecoilGrid::GetPointByKey(FGuid Key)
{
	static FRecoilPoint RecoilPoint;

	for (int32 i = 0; i < RecoilPoints.Num(); i++)
	{
		if (RecoilPoints[i].Hash == Key)
		{
			return RecoilPoints[i];
		}
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

FRecoilPoint::FRecoilPoint()
{
	PointCoord = FVector2D::ZeroVector;
	Hash.Invalidate();
}
