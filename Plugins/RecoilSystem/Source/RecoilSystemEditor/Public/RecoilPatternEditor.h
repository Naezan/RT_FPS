// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Toolkits/AssetEditorToolkit.h"
#include "Toolkits/IToolkitHost.h"

/**
 * 
 */
class RECOILSYSTEMEDITOR_API RecoilPatternEditor : public FAssetEditorToolkit
{
public:
	RecoilPatternEditor();
	
	virtual void InitRecoilPatternEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class URecoilPatternAsset* RecoilPattern);

	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager) override;

	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;

	TSharedRef<SDockTab> SpawnTab_RecoilWidget(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_Details(const FSpawnTabArgs& Args);
	TSharedRef<SDockTab> SpawnTab_PointDetails(const FSpawnTabArgs& Args);

private:
	const TSharedRef<FTabManager::FLayout> CreateEditorLayout();

private:
	// 디테일 뷰
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<class IStructureDetailsView> SelectedPointDetailsView;

	// 리코일패턴 뷰포트
	TSharedPtr<class SRecoilViewportWidget> RecoilPatternWidget;

	static const FName EditorAppIdentifier;

	static const FName RecoilWidgetTabId;
	static const FName DetailsTabId;
	static const FName PointDetailsTabId;
};
