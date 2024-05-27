// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SCurveEditor.h"

class RecoilPatternEditor;
class RecoilBackgroundPanel;

/**
 * 
 */
class RECOILSYSTEMEDITOR_API SRecoilViewportWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRecoilViewportWidget)
	{}

	// 생성시 전달할 인자
	SLATE_ARGUMENT(TSharedPtr<RecoilPatternEditor>, RecoilEditor)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 BaseLayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	virtual FReply OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual FReply OnKeyUp(const FGeometry& InMyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& InMouseEvent) override;

private:
	void PaintDragBox(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	bool IsDragging() const;
	bool IsDragSet() const;
	void ResetDrag();


protected:
	/** The location of mouse during the last OnMouseButtonDown callback in widget local coordinates. */
	FVector2D MouseDownLocation;

	/** The location of the mouse during the last OnMouseMove callback in widget local coordinates. */
	FVector2D MouseMoveLocation;

private:
	TWeakPtr<RecoilPatternEditor> RecoilEditor;

	TSharedPtr<RecoilBackgroundPanel> BackgroundPanel;
};
