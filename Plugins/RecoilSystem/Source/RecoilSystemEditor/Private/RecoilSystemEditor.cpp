#include "RecoilSystemEditor.h"
#include "IAssetTools.h"
#include "AssetTypeActions_RecoilPatternAsset.h"
#include "RecoilPatternEditor.h"
#include "RecoilEditorCommands.h"

DEFINE_LOG_CATEGORY(RecoilSystemEditor);

#define LOCTEXT_NAMESPACE "FRecoilSystemEditor"

void FRecoilSystemEditor::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	
	//Add asset category
	const auto AssetCategory = AssetTools.RegisterAdvancedAssetCategory(
		FName(TEXT("RecoilSystem")), LOCTEXT("RecoilAsset", "RecoilSystem"));
	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_RecoilPatternAsset(AssetCategory)));
}

void FRecoilSystemEditor::ShutdownModule()
{
	FRecoilEditorCommands::Unregister();
}

TSharedRef<class RecoilPatternEditor> FRecoilSystemEditor::CreateRecoilPatternEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, URecoilPatternAsset* RecoilPattern)
{
	TSharedRef<RecoilPatternEditor> RecoilEditor(new RecoilPatternEditor());

	RecoilEditor->InitRecoilPatternEditor(Mode, InitToolkitHost, RecoilPattern);

	return RecoilEditor;
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FRecoilSystemEditor, RecoilSystemEditor)