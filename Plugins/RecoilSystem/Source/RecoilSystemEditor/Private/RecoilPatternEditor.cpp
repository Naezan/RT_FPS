// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilPatternEditor.h"
#include "RecoilPatternAsset.h"
#include "RecoilEditorCommands.h"
#include "RecoilGrid.h"

#include "SRecoilViewportWidget.h"
#include "IStructureDetailsView.h"
#include "Framework/Docking/TabManager.h"

#define LOCTEXT_NAMESPACE "RecoilPatternEditor"

const FName RecoilPatternEditor::EditorAppIdentifier("RecoilPatternEditorApp");

const FName RecoilPatternEditor::RecoilWidgetTabId("RecoilPattern Widget");
const FName RecoilPatternEditor::DetailsTabId("RecoilPattern Details");
const FName RecoilPatternEditor::PointDetailsTabId("RecoilPattern PointDetails");

RecoilPatternEditor::RecoilPatternEditor()
{
}

void RecoilPatternEditor::InitRecoilPatternEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, URecoilPatternAsset* RecoilPattern)
{
	FRecoilEditorCommands::Register();
	// 명령어 리스트 생성
	CommandList = MakeShareable(new FUICommandList);
	CommandList->MapAction(FRecoilEditorCommands::Get().AddDefaultPoint, FExecuteAction::CreateSP(this, &RecoilPatternEditor::AddPoint), FCanExecuteAction());
	CommandList->MapAction(FRecoilEditorCommands::Get().DeleteSelectedPoint, FExecuteAction::CreateSP(this, &RecoilPatternEditor::RemovePoint), FCanExecuteAction::CreateSP(this, &RecoilPatternEditor::CanRemovePoint));

	// 반동 위젯 생성
	RecoilPatternWidget = SNew(SRecoilViewportWidget)
		.RecoilEditor(SharedThis(this));

	// 에디터 레이아웃 셋팅
	const TSharedRef<FTabManager::FLayout> EditorDefaultLayout = CreateEditorLayout();

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, EditorAppIdentifier, EditorDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, RecoilPattern);

}

void RecoilPatternEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_RecoilPatternEditor", "Recoil Pattern Editor"));
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(RecoilWidgetTabId, FOnSpawnTab::CreateSP(this, &RecoilPatternEditor::SpawnTab_RecoilWidget))
		.SetDisplayName(LOCTEXT("RecoilPatternGraph", "Recoil Pattern Graph"));

	{
		FDetailsViewArgs DetailsViewArgs;
		DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::HideNameArea;
		DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
		InTabManager->RegisterTabSpawner(DetailsTabId, FOnSpawnTab::CreateSP(this, &RecoilPatternEditor::SpawnTab_Details))
			.SetDisplayName(LOCTEXT("RecoilEditorDetail", "Recoil Editor Detail"));
	}
	
	
	{
		SelectedPointDetailsView = PropertyEditorModule.CreateStructureDetailView(FDetailsViewArgs(), FStructureDetailsViewArgs(), nullptr);
		InTabManager->RegisterTabSpawner(PointDetailsTabId, FOnSpawnTab::CreateSP(this, &RecoilPatternEditor::SpawnTab_PointDetails))
			.SetDisplayName(LOCTEXT("RecoilPointDetail", "Recoil Point Detail"));
	}
}

void RecoilPatternEditor::UnregisterTabSpawners(const TSharedRef<class FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(RecoilWidgetTabId);
	InTabManager->UnregisterTabSpawner(DetailsTabId);
	InTabManager->UnregisterTabSpawner(PointDetailsTabId);
}

FName RecoilPatternEditor::GetToolkitFName() const
{
	return FName("RecoilPatternEditor");
}

FText RecoilPatternEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "RecoilPatternEditor");
}

FString RecoilPatternEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "RecoilPattern ").ToString();
}

FLinearColor RecoilPatternEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(1.f, 0.65f, 0.f);
}

TSharedRef<SDockTab> RecoilPatternEditor::SpawnTab_RecoilWidget(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == RecoilWidgetTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("RecoilPatternWidget", "RecoilPattern Widget"))
		[
			RecoilPatternWidget.ToSharedRef()
		];
}

TSharedRef<SDockTab> RecoilPatternEditor::SpawnTab_Details(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == DetailsTabId);

	DetailsView->SetObject(GetEditingObject());

	return SNew(SDockTab)
		.Label(LOCTEXT("RecoilPatternDetails", "RecoilPattern Details"))
		[
			DetailsView.ToSharedRef()
		];
}

TSharedRef<SDockTab> RecoilPatternEditor::SpawnTab_PointDetails(const FSpawnTabArgs& Args)
{
	check(Args.GetTabId() == PointDetailsTabId);

	return SNew(SDockTab)
		.Label(LOCTEXT("RecoilPointDetails", "RecoilPoint Details"))
		[
			SelectedPointDetailsView->GetWidget().ToSharedRef()
		];
}

URecoilPatternAsset* RecoilPatternEditor::GetRecoilPatternAsset() const
{
	return CastChecked<URecoilPatternAsset>(GetEditingObject());
}

URecoilGrid* RecoilPatternEditor::GetRecoilGrid() const
{
	const URecoilPatternAsset* RecoilAsset = GetRecoilPatternAsset();
	return (RecoilAsset != nullptr) ? RecoilAsset->RecoilGrid : nullptr;
}

void RecoilPatternEditor::AddPoint()
{
	const FScopedTransaction Transaction(FText::FromString("Add Point"));

	GetRecoilGrid()->AddPoint(FRecoilPoint());

	//SelectedPoints.ClearSelection();
	//SelectedPoints.AddPoint(Index);
}

void RecoilPatternEditor::RemovePoint()
{
	const FScopedTransaction Transaction(FText::FromString("Remove Point"));

	/*for (int32 Index : SelectedPoints.GetAllPoints())
	{
		GetRecoilGrid()->RemovePoint(Index);
	}*/

	//SelectedPoints.ClearSelection();
}

bool RecoilPatternEditor::CanRemovePoint()
{
	return false;
}

const TSharedRef<FTabManager::FLayout> RecoilPatternEditor::CreateEditorLayout()
{
	return FTabManager::NewLayout("RecoilPatternEditor_Layout_v1")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				// 에디터 구성
				FTabManager::NewSplitter()->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()->SetOrientation(Orient_Vertical)
					->Split
					(
						// 뷰포트 추가
						FTabManager::NewStack()
						->SetSizeCoefficient(0.9f)
						->AddTab(RecoilWidgetTabId, ETabState::OpenedTab)
					)
					->Split
					(
						// 뷰포트 포인트 디테일 창 추가
						FTabManager::NewStack()
						->AddTab(PointDetailsTabId, ETabState::OpenedTab)
					)
				)
				->Split
				(
					// 뷰포트 디테일 창 추가
					FTabManager::NewStack()
					->SetSizeCoefficient(0.3f)
					->AddTab(DetailsTabId, ETabState::OpenedTab)
				)
			)
		);
}

#undef LOCTEXT_NAMESPACE