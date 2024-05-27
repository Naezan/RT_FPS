// Fill out your copyright notice in the Description page of Project Settings.


#include "SRecoilViewportWidget.h"
#include "SlateOptMacros.h"
#include "RecoilBackgroundPanel.h"
#include "Widgets/SOverlay.h"
#include "Layout/Children.h"
#include "Layout/Visibility.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SRecoilViewportWidget::Construct(const FArguments& InArgs)
{
	check(InArgs._RecoilEditor.IsValid());
	RecoilEditor = InArgs._RecoilEditor;

	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SAssignNew(BackgroundPanel, RecoilBackgroundPanel)
						.Visibility(EVisibility::HitTestInvisible)
				]
		];
}
int32 SRecoilViewportWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	LayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	//DrawPoints(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
	//DrawPointNumbers(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);

	if (IsDragging())
	{
		PaintDragBox(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
	}

	return LayerId;
}
FReply SRecoilViewportWidget::OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	ResetDrag();

	const bool bLeftMouseButton = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;

	if (bLeftMouseButton)
	{
		MouseDownLocation = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		MouseMoveLocation = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
	}

	return FReply::Handled();
}
FReply SRecoilViewportWidget::OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsDragging() && IsDragSet())
	{
		ResetDrag();
		return FReply::Handled();
	}

	ResetDrag();

	return FReply::Handled();
}
FReply SRecoilViewportWidget::OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const bool bLeftMouseButton = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;

	if (IsDragging() || bLeftMouseButton)
	{
		MouseMoveLocation = InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());

		//TODO Rect안에 있는지 체크 후, Selection배열에 추가
	}

	return SCompoundWidget::OnMouseMove(InMyGeometry, InMouseEvent);
}
FReply SRecoilViewportWidget::OnMouseWheel(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	return BackgroundPanel->OnMouseWheel(InMyGeometry, InMouseEvent);
}

void SRecoilViewportWidget::OnMouseLeave(const FPointerEvent& InMouseEvent)
{
	SCompoundWidget::OnMouseLeave(InMouseEvent);

	ResetDrag();
}

void SRecoilViewportWidget::PaintDragBox(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId) const
{
	FVector2D MarqueTopLeft
	(
		FMath::Min(MouseDownLocation.X, MouseMoveLocation.X),
		FMath::Min(MouseDownLocation.Y, MouseMoveLocation.Y)
	);

	FVector2D MarqueBottomRight
	(
		FMath::Max(MouseDownLocation.X, MouseMoveLocation.X),
		FMath::Max(MouseDownLocation.Y, MouseMoveLocation.Y)
	);

	const FSlateRect SelectionRect(MarqueTopLeft, MarqueBottomRight);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(SelectionRect.GetTopLeft(), SelectionRect.GetSize()),
		FAppStyle::GetBrush(TEXT("MarqueeSelection"))
	);
}

bool SRecoilViewportWidget::IsDragging() const
{
	return !MouseDownLocation.IsNearlyZero() || !MouseMoveLocation.IsNearlyZero();
}

bool SRecoilViewportWidget::IsDragSet() const
{
	return !MouseDownLocation.IsNearlyZero() && !MouseMoveLocation.IsNearlyZero();
}

void SRecoilViewportWidget::ResetDrag()
{
	MouseDownLocation = FVector2D::ZeroVector;
	MouseMoveLocation = FVector2D::ZeroVector;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION