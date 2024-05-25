// Fill out your copyright notice in the Description page of Project Settings.


#include "SRecoilViewportWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRecoilViewportWidget::Construct(const FArguments& InArgs)
{
	check(InArgs._RecoilEditor.IsValid());
	RecoilEditor = InArgs._RecoilEditor;

	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION
