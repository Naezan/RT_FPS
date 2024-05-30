// Fill out your copyright notice in the Description page of Project Settings.


#include "SRecoilViewportWidget.h"
#include "SlateOptMacros.h"
#include "RecoilPatternEditor.h"
#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"
#include "RecoilBackgroundPanel.h"
#include "RecoilEditorCommands.h"

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

	if (DragBoxEvent.IsDragging())
	{
		PaintDragBox(AllottedGeometry, MyCullingRect, OutDrawElements, ++LayerId);
	}

	return LayerId;
}
FReply SRecoilViewportWidget::OnMouseButtonDown(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	DragBoxEvent.ResetDrag();

	const bool bLeftMouseButton = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	const bool bRightMouseButton = InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton;

	if (bLeftMouseButton)
	{
		// 포인트 위치에서는 드래그 불가능
		const FRecoilPoint FoundPoint = FindPointUnderMouse(InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
		if (!FoundPoint.Hash.IsValid())
		{
			bStartDragPoint = false;

			DragBoxEvent = FViewportMouseEvent(
				InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()),
				InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
		}
		else
		{
			bStartDragPoint = true;

			// 포인트 추가
			if (!RecoilEditor.Pin()->HasSelectedPoint(FoundPoint.Hash))
			{
				RecoilEditor.Pin()->ClearSelectedPoints();

				RecoilEditor.Pin()->AddSelectedPoint(FoundPoint.Hash);
			}
		}
	}
	else if (bRightMouseButton)
	{
		ViewportDragEvent = FViewportMouseEvent(
			InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()),
			InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
	}

	return FReply::Handled();
}
FReply SRecoilViewportWidget::OnMouseButtonUp(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (ViewportDragEvent.IsMoveDrag())
	{
		ViewportDragEvent.ResetDrag();

		return FReply::Handled();
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bStartDragPoint = false;
	}
	// Create add point menu
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (RecoilEditor.IsValid())
		{
			if (!RecoilEditor.Pin()->HasAnySelectedPoints())
			{
				RecoilEditor.Pin()->ClearSelectedPoints();

				const FRecoilPoint FoundPoint = FindPointUnderMouse(InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
				if (FoundPoint.Hash.IsValid())
				{
					RecoilEditor.Pin()->AddSelectedPoint(FoundPoint.Hash);
				}
			}
		}

		FMenuBuilder MenuBuilder(/*bShouldCloseWindowAfterMenuSelection=*/ true, /*CommandList=*/ RecoilEditor.Pin()->GetCommandList());

		MenuBuilder.BeginSection("RecoilGraph", FText::FromString(TEXT("Recoil Options")));
		{
			MenuBuilder.AddMenuEntry(FRecoilEditorCommands::Get().AddDefaultPoint, NAME_None);
			MenuBuilder.AddMenuEntry(FRecoilEditorCommands::Get().DeleteSelectedPoint, NAME_None);
		}
		MenuBuilder.EndSection();

		FSlateApplication::Get().PushMenu(
			AsShared(),
			FWidgetPath(),
			MenuBuilder.MakeWidget(),
			FSlateApplication::Get().GetCursorPos(),
			FPopupTransitionEffect(FPopupTransitionEffect::ContextMenu));
	}

	DragBoxEvent.ResetDrag();
	ViewportDragEvent.ResetDrag();

	return FReply::Handled();
}
FReply SRecoilViewportWidget::OnMouseMove(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	const bool bLeftMouseButton = InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
	const bool bRightMouseButton = InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton;

	if (DragBoxEvent.IsDragging() || bLeftMouseButton)
	{
		DragBoxEvent.SetMoveLocation(InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));

		FVector2D DragTopLeft
		(
			FMath::Min(DragBoxEvent.MouseDownLocation.X, DragBoxEvent.MouseMoveLocation.X),
			FMath::Min(DragBoxEvent.MouseDownLocation.Y, DragBoxEvent.MouseMoveLocation.Y)
		);

		FVector2D DragBottomRight
		(
			FMath::Max(DragBoxEvent.MouseDownLocation.X, DragBoxEvent.MouseMoveLocation.X),
			FMath::Max(DragBoxEvent.MouseDownLocation.Y, DragBoxEvent.MouseMoveLocation.Y)
		);

		const FSlateRect DragBoxRect(DragTopLeft, DragBottomRight);

		if (const URecoilPatternAsset* RecoilAsset = GetRecoilAsset())
		{
			if (RecoilEditor.IsValid())
			{
				RecoilEditor.Pin()->ClearSelectedPoints();

				for (int32 i = 0; i < RecoilAsset->GetRecoilPointsNum(); i++)
				{
					const FRecoilPoint RecoilPoint = RecoilAsset->GetRecoilPointAt(i);
					if (FSlateRect::DoRectanglesIntersect(DragBoxRect, GetPointRect(RecoilPoint)))
					{
						RecoilEditor.Pin()->AddSelectedPoint(RecoilPoint.Hash);
					}
				}
			}
		}
	}
	
	// 포인트 이동
	if (bStartDragPoint || bLeftMouseButton)
	{
		FVector2D MoveLocDelta = LocalToAbsolutePoint(InMouseEvent.GetScreenSpacePosition()) - LocalToAbsolutePoint(InMouseEvent.GetLastScreenSpacePosition());

		if (const URecoilPatternAsset* RecoilAsset = GetRecoilAsset())
		{
			for (int32 i = 0; i < RecoilAsset->GetRecoilPointsNum(); i++)
			{
				FRecoilPoint& RecoilPoint = const_cast<FRecoilPoint&>(RecoilAsset->GetRecoilPointAt(i));

				if (RecoilEditor.Pin()->HasSelectedPoint(RecoilPoint.Hash))
				{
					RecoilPoint.PointCoord += MoveLocDelta;
				}
			}
		}
	}

	if (ViewportDragEvent.IsDragging() || bRightMouseButton)
	{
		ViewportDragEvent.SetMoveLocation(InMyGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));

		const FVector2D CursorDelta = InMouseEvent.GetCursorDelta();
		const FVector2D CurViewOffset = BackgroundPanel->GetViewOffset() - CursorDelta / BackgroundPanel->GetZoomAmount();
		BackgroundPanel->SetViewOffset(CurViewOffset);
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

	bStartDragPoint = false;
	DragBoxEvent.ResetDrag();
}

URecoilPatternAsset* SRecoilViewportWidget::GetRecoilAsset() const
{
	return RecoilEditor.IsValid() ? RecoilEditor.Pin()->GetRecoilPatternAsset() : nullptr;
}

TArray<FGuid>& SRecoilViewportWidget::GetSelectedRecoilPoints()
{
	check(RecoilEditor.IsValid());

	return RecoilEditor.Pin()->GetSelectedPoints();
}

void SRecoilViewportWidget::PaintPoints(const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle) const
{
	const URecoilPatternAsset* RecoilAsset = GetRecoilAsset();
	URecoilGrid* RecoilGrid = RecoilAsset->GetRecoilGrid();

	for (int32 i = 0; i < RecoilGrid->GetPointsNum(); i++)
	{
		const FRecoilPoint& RecoilPoint = RecoilGrid->GetPoint(i);
		FVector2D TopLeftPoint = GetPointTopLeftLocation(RecoilPoint.PointCoord);
		FVector2D CenterPoint = GetPointCenterLocation(RecoilPoint.PointCoord);

		// Draw Points
		{
			const bool bIsSelected = RecoilEditor.Pin()->HasSelectedPoint(RecoilPoint.Hash);
			const FSlateBrush* PointBrush = bIsSelected ? FAppStyle::GetBrush("CurveEd.CurveKeySelected") : FAppStyle::GetBrush("CurveEd.CurveKey");

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(TopLeftPoint, FVector2D(FRecoilPoint::PointSize)),
				PointBrush,
				ESlateDrawEffect::None,
				PointBrush->GetTint(InWidgetStyle));
		}

		// Draw Point Texts
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

		// Draw Point line
		if (i > 0)
		{
			const FRecoilPoint& PrevRecoilPoint = RecoilGrid->GetPoint(i - 1);

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				AllottedGeometry.ToPaintGeometry(),
				{
					GetPointCenterLocation(PrevRecoilPoint.PointCoord),
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
		FMath::Min(DragBoxEvent.MouseDownLocation.X, DragBoxEvent.MouseMoveLocation.X),
		FMath::Min(DragBoxEvent.MouseDownLocation.Y, DragBoxEvent.MouseMoveLocation.Y)
	);

	FVector2D MarqueBottomRight
	(
		FMath::Max(DragBoxEvent.MouseDownLocation.X, DragBoxEvent.MouseMoveLocation.X),
		FMath::Max(DragBoxEvent.MouseDownLocation.Y, DragBoxEvent.MouseMoveLocation.Y)
	);

	const FSlateRect SelectionRect(MarqueTopLeft, MarqueBottomRight);

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(SelectionRect.GetTopLeft(), SelectionRect.GetSize()),
		FAppStyle::GetBrush(TEXT("MarqueeSelection"))
	);
}

const FRecoilPoint& SRecoilViewportWidget::FindPointUnderMouse(FVector2D MousePosition)
{
	static FRecoilPoint TempRecoilPoint;

	const FSlateRect MouseRect = FSlateRect::FromPointAndExtent(MousePosition - (FRecoilPoint::PointSize / 2.f), FVector2D(FRecoilPoint::PointSize));

	const URecoilPatternAsset* RecoilAsset = GetRecoilAsset();
	for (int32 i = 0; i < RecoilAsset->GetRecoilPointsNum(); i++)
	{
		const FRecoilPoint& RecoilPoint = RecoilAsset->GetRecoilPointAt(i);
		FVector2D CenterPoint = GetPointCenterLocation(RecoilPoint.PointCoord);

		if (MouseRect.ContainsPoint(CenterPoint))
		{
			return RecoilPoint;
		}
	}

	return TempRecoilPoint;
}

FSlateRect SRecoilViewportWidget::GetPointRect(const FRecoilPoint& InPoint) const
{
	FVector2D CenterLocation = GetPointCenterLocation(InPoint.PointCoord);
	return FSlateRect::FromPointAndExtent(CenterLocation - (FRecoilPoint::PointSize / 2.f), FVector2D(FRecoilPoint::PointSize));
}

FVector2D SRecoilViewportWidget::GetPointTopLeftLocation(const FVector2D& InPoint) const
{
	FVector2D CenterLocation = GetPointCenterLocation(InPoint);
	return CenterLocation - FRecoilPoint::PointSize / 2.f;
}

FVector2D SRecoilViewportWidget::GetPointCenterLocation(const FVector2D& InPoint) const
{
	FVector2D InverseLocation = InPoint;
	InverseLocation.Y *= -1.f;

	return BackgroundPanel->GraphCoordToPanelCoord(InverseLocation * 16.f);
}

FVector2D SRecoilViewportWidget::LocalToAbsolutePoint(const FVector2D& InPoint) const
{
	FVector2D InverseLocation = BackgroundPanel->PanelCoordToGraphCoord(InPoint) / 16.f;
	InverseLocation.Y *= -1.f;
	return InverseLocation;
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

bool FViewportMouseEvent::IsDragging() const
{
	return !MouseDownLocation.IsNearlyZero() || !MouseMoveLocation.IsNearlyZero();
}

bool FViewportMouseEvent::IsValidDrag() const
{
	return !MouseDownLocation.IsNearlyZero() && !MouseMoveLocation.IsNearlyZero();
}

bool FViewportMouseEvent::IsMoveDrag() const
{
	return !MouseDownLocation.Equals(MouseMoveLocation);
}

void FViewportMouseEvent::ResetDrag()
{
	MouseDownLocation = FVector2D::ZeroVector;
	MouseMoveLocation = FVector2D::ZeroVector;
}
