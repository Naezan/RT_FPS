// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "SCurveEditor.h"

class RecoilPatternEditor;
class RecoilBackgroundPanel;

struct FViewportMouseEvent
{
public:
	FViewportMouseEvent() :
		MouseDownLocation(FVector2D::ZeroVector), MouseMoveLocation(FVector2D::ZeroVector)
	{}
	FViewportMouseEvent(FVector2D MouseDownLoc, FVector2D MouseMoveLoc) :
		MouseDownLocation(MouseDownLoc), MouseMoveLocation(MouseMoveLoc)
	{}

	bool IsDragging() const;
	bool IsValidDrag() const;
	bool IsMoveDrag() const;
	void ResetDrag();

	void SetMoveLocation(FVector2D InMoveLocation) { MouseMoveLocation = InMoveLocation; }

	/** The location of mouse during the last OnMouseButtonDown callback in widget local coordinates. */
	FVector2D MouseDownLocation;

	/** The location of the mouse during the last OnMouseMove callback in widget local coordinates. */
	FVector2D MouseMoveLocation;
};

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

public:
	class URecoilPatternAsset* GetRecoilAsset() const;

private:
	void PaintPoints(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle) const;
	void PaintDragBox(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const;

	FVector2D GetPointTopLeftLocation(const struct FRecoilPoint& InPoint) const;
	FVector2D GetPointCenterLocation(const struct FRecoilPoint& InPoint) const;

protected:
	FViewportMouseEvent DragBoxEvent;
	FViewportMouseEvent ViewportDragEvent;

private:
	TWeakPtr<RecoilPatternEditor> RecoilEditor;

	TSharedPtr<RecoilBackgroundPanel> BackgroundPanel;
};
