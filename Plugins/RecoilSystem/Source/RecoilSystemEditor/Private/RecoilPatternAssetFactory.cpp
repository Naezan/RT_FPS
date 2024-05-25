// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilPatternAssetFactory.h"
#include "RecoilPatternAsset.h"

URecoilPatternAssetFactory::URecoilPatternAssetFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = URecoilPatternAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}

UObject* URecoilPatternAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<URecoilPatternAsset>(InParent, InClass, InName, Flags);;
}

FText URecoilPatternAssetFactory::GetToolTip() const
{
	return NSLOCTEXT("RecoilSystem", "RecoilPatternFactoryDescription", "RecoilPattern to firing");
}
