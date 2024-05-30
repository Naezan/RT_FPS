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

	URecoilPatternAsset* GetRecoilPatternAsset() const;
	class URecoilGrid* GetRecoilGrid() const;
	TSharedRef<class FUICommandList> GetCommandList() const;
	TArray<FGuid>& GetSelectedPoints();

	void AddSelectedPoint(FGuid IndexKey);
	void ClearSelectedPoints();
	bool HasAnySelectedPoints() const;
	bool HasSelectedPoint(FGuid IndexKey) const;

protected:
	void AddPoint();
	void RemovePoint();
	bool CanRemovePoint();
	void OnFinishedSelectedPoint(const FPropertyChangedEvent& PropertyChangedEvent);
	void OnSelectPointChanged();

private:
	const TSharedRef<FTabManager::FLayout> CreateEditorLayout();

private:
	// ������ ��
	TSharedPtr<class IDetailsView> DetailsView;
	TSharedPtr<class IStructureDetailsView> SelectedPointDetailsView;
	TSharedPtr<class FStructOnScope> SelectedPointDetailsScope;

	// ���������� ����Ʈ
	TSharedPtr<class SRecoilViewportWidget> RecoilPatternWidget;

	// �ּ� �����Ϳ� ��ɾ�
	TSharedRef<class FUICommandList> CommandList;

	TArray<FGuid> SelectedPoints;

	static const FName EditorAppIdentifier;

	static const FName RecoilWidgetTabId;
	static const FName DetailsTabId;
	static const FName PointDetailsTabId;
};
