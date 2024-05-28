// Fill out your copyright notice in the Description page of Project Settings.


#include "SRecoilViewportWidget.h"
#include "SlateOptMacros.h"
#include "RecoilPatternEditor.h"
#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"
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

	PaintPoints(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId, InWidgetStyle);

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

URecoilPatternAsset* SRecoilViewportWidget::GetRecoilAsset() const
{
	return RecoilEditor.IsValid() ? RecoilEditor.Pin()->GetRecoilPatternAsset() : nullptr;
}

void SRecoilViewportWidget::PaintPoints(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle) const
{
	const URecoilPatternAsset* RecoilAsset = GetRecoilAsset();
	URecoilGrid* RecoilGrid = RecoilAsset->GetRecoilGrid();
	
	for (int32 i = 0; i < RecoilGrid->GetPointsNum(); i++)
	{
		const FRecoilPoint& RecoilPoint = RecoilGrid->GetPoint(i);
		FVector2D TopLeftPoint = GetPointTopLeftLocation(RecoilPoint);
		FVector2D CenterPoint = GetPointCenterLocation(RecoilPoint);
		
		{
			const bool bIsSelected = false; /*GetUnitsSelection().HasSelectionUnit(recoilUnit.Id);*/
			const FSlateBrush* PointBrush = bIsSelected ? FAppStyle::GetBrush("CurveEd.CurveKeySelected") : FAppStyle::GetBrush("CurveEd.CurveKey");

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(TopLeftPoint, FVector2D(FRecoilPoint::PointSize)),
				PointBrush,
				ESlateDrawEffect::None,
				PointBrush->GetTint(InWidgetStyle));
		}
		
		{
			FVector2D NumberDrawPoint = TopLeftPoint;
			NumberDrawPoint.X += FRecoilPoint::PointSize * 2;

			FFontOutlineSettings OutlineSettings;
			OutlineSettings.OutlineColor = FLinearColor::Black;
			OutlineSettings.OutlineSize = 2;
			FSlateDrawElement::MakeText(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(NumberDrawPoint, FVector2D(FRecoilPoint::PointSize)),
				FString::FromInt(i),
				FCoreStyle::GetDefaultFontStyle("Bold", 8),
				ESlateDrawEffect::None,
				FLinearColor::White);
		}

		if(i > 0)
		{
			const FRecoilPoint& PrevRecoilPoint = RecoilGrid->GetPoint(i - 1);

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				{
					GetPointCenterLocation(PrevRecoilPoint),
					CenterPoint,
				},
				ESlateDrawEffect::None,
				FLinearColor::White,
				true,
				2.f);
		}
	}
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

FVector2D SRecoilViewportWidget::GetPointTopLeftLocation(const FRecoilPoint& InPoint) const
{
	FVector2D CenterLocation = GetPointCenterLocation(InPoint);
	return CenterLocation - FRecoilPoint::PointSize / 2.f;
}

FVector2D SRecoilViewportWidget::GetPointCenterLocation(const FRecoilPoint& InPoint) const
{
	FVector2D InverseLocation = InPoint.PointCoord;
	InverseLocation.Y *= -1.f;

	return BackgroundPanel->GraphCoordToPanelCoord(InverseLocation * 16.f);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION