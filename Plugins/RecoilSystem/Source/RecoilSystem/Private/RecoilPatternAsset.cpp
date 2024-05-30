// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"

URecoilPatternAsset::URecoilPatternAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// RF_Public : 에디터에서 저장하기 위해 필요함
	// RF_Transactional : Undo, Redo
	RecoilGrid = NewObject<URecoilGrid>(this, "RecoilGrid", RF_Public | RF_Transactional);
}

URecoilGrid* URecoilPatternAsset::GetRecoilGrid() const
{
	return RecoilGrid;
}

int32 URecoilPatternAsset::GetRecoilPointsNum() const
{
	return RecoilGrid ? RecoilGrid->GetPointsNum() : 0;
}

FRecoilPoint URecoilPatternAsset::GetRecoilPointAt(int32 RecoilIndex) const
{
	return RecoilGrid ? RecoilGrid->GetPoint(RecoilIndex) : FRecoilPoint();
}
