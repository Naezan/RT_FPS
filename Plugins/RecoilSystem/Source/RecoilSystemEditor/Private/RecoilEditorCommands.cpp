// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilEditorCommands.h"

#define LOCTEXT_NAMESPACE "RecoilEditorCommands"

void FRecoilEditorCommands::RegisterCommands()
{
	UI_COMMAND(AddDefaultPoint, "Add point", "Add point to (0, 0)", EUserInterfaceActionType::Button, FInputChord());
	UI_COMMAND(DeleteSelectedPoint, "Delete point", "Delete selected point", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE