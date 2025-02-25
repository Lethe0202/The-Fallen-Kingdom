// Fill out your copyright notice in the Description page of Project Settings.


#include "ScrollableWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"


void UScrollableWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UScrollableWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UScrollableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		bIsDragging = true;
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::RightMouseButton).NativeReply;
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UScrollableWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging)
	{
		if (ScrollableSizeBox)
		{
			if (UCanvasPanelSlot* ScrollableSizeBoxSlot= Cast<UCanvasPanelSlot>(ScrollableSizeBox->Slot))
			{
				ScrollableSizeBoxSlot->SetPosition(ScrollableSizeBoxSlot->GetPosition() + InMouseEvent.GetCursorDelta());
				RefreshSizeBox();
			}
		}
	}
	
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UScrollableWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UScrollableWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bIsDragging = false;
		return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	}
	
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UScrollableWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (UCanvasPanelSlot* ScrollableSizeBoxSlot = Cast<UCanvasPanelSlot>(ScrollableSizeBox->Slot))
	{
		float WidgetMovementDelta = InMouseEvent.GetWheelDelta() * 0.1f;
		
		CurScale = FMath::Clamp(CurScale + WidgetMovementDelta, 0.f, 1.f);
		float WidgetSize = FMath::Lerp(MinWidgetSize, MaxWidgetSize, CurScale);
		ScrollableSizeBoxSlot->SetSize(FVector2D(WidgetSize, WidgetSize));
		
		RefreshSizeBox();
	}
	
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UScrollableWidget::RefreshSizeBox()
{
	if (UCanvasPanelSlot* ScrollableSizeBoxSlot= Cast<UCanvasPanelSlot>(ScrollableSizeBox->Slot))
	{
		FVector2D WidgetSize = ScrollableSizeBoxSlot->GetSize();
		FVector2D WidgetPosition = ScrollableSizeBoxSlot->GetPosition();
		
		float AbsWidgetPositionX = FMath::Abs(WidgetPosition.X);
		float AbsWidgetPositionY = FMath::Abs(WidgetPosition.Y);
		
		//현 프로젝트는 스크롤뷰의 앵커가 0.5(중앙)기준으로 정의하기때문에 Size / 2 로 계산
		if (AbsWidgetPositionX >= (WidgetSize.X - MinWidgetSize) / 2)
		{
			WidgetPosition.X = (WidgetSize.X - MinWidgetSize) / 2 * FMath::Sign(WidgetPosition.X);
		}
		
		if (AbsWidgetPositionY >= (WidgetSize.Y - MinWidgetSize) / 2)
		{
			WidgetPosition.Y = (WidgetSize.Y - MinWidgetSize) / 2 * FMath::Sign(WidgetPosition.Y);
		}
		
		ScrollableSizeBoxSlot->SetPosition(WidgetPosition);
	}
}
