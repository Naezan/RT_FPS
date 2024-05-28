// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"

URecoilPatternAsset::URecoilPatternAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// RF_Public : �����Ϳ��� �����ϱ� ���� �ʿ���
	// RF_Transactional : Undo, Redo
	RecoilGrid = NewObject<URecoilGrid>(this, "RecoilGrid", RF_Public | RF_Transactional);
}

URecoilGrid* URecoilPatternAsset::GetRecoilGrid() const
{
	return RecoilGrid;
}
