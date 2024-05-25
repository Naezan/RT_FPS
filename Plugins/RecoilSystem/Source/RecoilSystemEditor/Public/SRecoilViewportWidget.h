// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class RecoilPatternEditor;

/**
 * 
 */
class RECOILSYSTEMEDITOR_API SRecoilViewportWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRecoilViewportWidget)
	{}

	// ������ ������ ����
	SLATE_ARGUMENT(TSharedPtr<RecoilPatternEditor>, RecoilEditor)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

private:
	TWeakPtr<RecoilPatternEditor> RecoilEditor;
};
