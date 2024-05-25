// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AssetTypeActions_Base.h"
#include "CoreMinimal.h"
#include "Toolkits/IToolkitHost.h"

class RECOILSYSTEMEDITOR_API FAssetTypeActions_RecoilPatternAsset : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_RecoilPatternAsset(EAssetTypeCategories::Type assetCategory);

public:
	// Name that actually appears in the editor
	virtual FText GetName() const override;
	virtual FColor GetTypeColor() const override;
	// Editor thumbnail image
	//virtual TSharedPtr<class SWidget> GetThumbnailOverlay(const FAssetData& AssetData) const override;
	virtual uint32 GetCategories() override;
	virtual EThumbnailPrimType GetDefaultThumbnailPrimitiveType(UObject* Asset) const override;
	virtual UClass* GetSupportedClass() const override;
	// Function that specify an editor that opens when double-clicking Asset
	//virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<class IToolkitHost> EditWithinLevelEditor = TSharedPtr<IToolkitHost>()) override;

private:
	EAssetTypeCategories::Type AssetCategory = EAssetTypeCategories::Misc;
};
