// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"

URecoilPatternAsset::URecoilPatternAsset(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	RecoilGrid = NewObject<URecoilGrid>(this, "RecoilGrid", RF_Transactional);
}
