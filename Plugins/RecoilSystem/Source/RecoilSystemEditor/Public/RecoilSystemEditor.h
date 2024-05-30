#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(RecoilSystemEditor, All, All);

class FRecoilSystemEditor : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

	virtual TSharedRef<class RecoilPatternEditor> CreateRecoilPatternEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, class URecoilPatternAsset* RecoilPattern);
};