// Fill out your copyright notice in the Description page of Project Settings.

#include "AssetTypeActions_RecoilPatternAsset.h"
#include "RecoilPatternAsset.h"

FAssetTypeActions_RecoilPatternAsset::FAssetTypeActions_RecoilPatternAsset(EAssetTypeCategories::Type assetCategory)
	: AssetCategory(assetCategory)
{
}

FText FAssetTypeActions_RecoilPatternAsset::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_RecoilPatternAsset", "RecoilPatternAsset");
}

FColor FAssetTypeActions_RecoilPatternAsset::GetTypeColor() const
{
	return FColor::Turquoise;
}

uint32 FAssetTypeActions_RecoilPatternAsset::GetCategories()
{
	return AssetCategory;
}

EThumbnailPrimType FAssetTypeActions_RecoilPatternAsset::GetDefaultThumbnailPrimitiveType(UObject* Asset) const
{
	return EThumbnailPrimType::TPT_Plane;
}

UClass* FAssetTypeActions_RecoilPatternAsset::GetSupportedClass() const
{
	return URecoilPatternAsset::StaticClass();
}