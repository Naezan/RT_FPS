// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "Styling/AppStyle.h"

/**
 * 
 */
class FRecoilEditorCommands : public TCommands<FRecoilEditorCommands>
{

public:
	FRecoilEditorCommands()
		: TCommands<FRecoilEditorCommands>(TEXT("RecoilEditor"),
			NSLOCTEXT("Contexts", "RecoilEditor", "RecoilEditor"),
			NAME_None,
			FAppStyle::GetAppStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> AddDefaultPoint;
	TSharedPtr<FUICommandInfo> DeleteSelectedPoint;
};
