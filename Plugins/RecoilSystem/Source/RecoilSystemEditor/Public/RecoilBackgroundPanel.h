// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SNodePanel.h"

/**
 * 
 */
class RECOILSYSTEMEDITOR_API RecoilBackgroundPanel : public SNodePanel
{
public:
	SLATE_BEGIN_ARGS(RecoilBackgroundPanel)
	{
	}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	using SNodePanel::GraphCoordToPanelCoord;
};
