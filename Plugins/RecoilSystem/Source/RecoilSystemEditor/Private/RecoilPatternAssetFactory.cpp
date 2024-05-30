// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilPatternAssetFactory.h"
#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"

URecoilPatternAssetFactory::URecoilPatternAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = URecoilPatternAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* URecoilPatternAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	URecoilPatternAsset* const RecoilAsset = NewObject<URecoilPatternAsset>(InParent, InClass, InName, Flags);
	URecoilGrid* const RecoilGridInfo = RecoilAsset->RecoilGrid;
	check(RecoilGridInfo != nullptr);

	RecoilGridInfo->AddPoint(FRecoilPoint(FVector2D(0.5f, 1.f)));
	RecoilGridInfo->AddPoint(FRecoilPoint(FVector2D(1.3f, 5.f)));
	RecoilGridInfo->AddPoint(FRecoilPoint(FVector2D(0.8f, 10.f)));

	return RecoilAsset;
}

FText URecoilPatternAssetFactory::GetToolTip() const
{
	return NSLOCTEXT("RecoilSystem", "RecoilPatternFactoryDescription", "RecoilPattern to firing");
}
