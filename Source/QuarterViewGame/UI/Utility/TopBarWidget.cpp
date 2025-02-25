// Fill out your copyright notice in the Description page of Project Settings.


#include "TopBarWidget.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UTopBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (bIsDragging)
	{
		FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		
		WidgetSize = TargetWidget->GetDesiredSize();		
		
		if (CanvasSlot)
		{
			//앵커 오프셋
			FAnchors Anchors = CanvasSlot->GetAnchors();
			FVector2D AnchorOffset = ViewportSize * Anchors.Minimum; //Maximum과 Minimum같다는 조건으로 Minimum으로 계산
			
			//피봇 오프셋
			FVector2D SlotPivot = CanvasSlot->GetAlignment();
			FVector2D PivotOffset = SlotPivot * WidgetSize;
			
			//드래그 오프셋 적용
			FVector2D NewPosition = MousePosition - DragOffset;
			
			//화면밖으로 Widget이 나가지않도록 조정
			//좌측
			if (NewPosition.X < 0)
			{
				NewPosition.X = 0;
			}
			//우측
			else if (NewPosition.X > ViewportSize.X - WidgetSize.X)
			{
				NewPosition.X = ViewportSize.X - WidgetSize.X;
			}
			
			//상단
			if (NewPosition.Y < 0)
			{
				NewPosition.Y = 0;
			}
			//하단
			else if (NewPosition.Y > ViewportSize.Y - WidgetSize.Y)
			{
				NewPosition.Y = ViewportSize.Y - WidgetSize.Y;
			}
			
			CanvasSlot->SetPosition(NewPosition + PivotOffset - AnchorOffset);
		}
	}
}

void UTopBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GEngine && GEngine->GameViewport)
	{
		//NativeConstruct에서 ViewportSize 초기화하면 Viewport가 완전히 초기화되기전에 실행될수있다. 때문에 게임이 시작하자마자 생성되는 Widget에서  Viewport를 사용할때는 다른곳에서 ViewportSize를 초기화 해야된다.
	}
	
	DragOffset = FVector2d::ZeroVector;
}

FReply UTopBarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UTopBarWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	
	if (OutOperation)
	{
		CanvasSlot = Cast<UCanvasPanelSlot>(TargetWidget->Slot);
		
		ViewportSize.X = GEngine->GameViewport->Viewport->GetSizeXY().X;
		ViewportSize.Y = GEngine->GameViewport->Viewport->GetSizeXY().Y;
		float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		ViewportSize /= ViewportScale;
		
		//ScreenSpacePosition
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
			
		// 타겟 위젯 위치
		FVector2D WidgetPosition = TargetWidget->GetCachedGeometry().GetAbsolutePosition();
		
		//마우스위치와 TargetWidget 오프셋
		DragOffset = MousePosition - WidgetPosition;
		
		//DPI 보정이 적용된 최종 오프셋
		DragOffset /= ViewportScale;
		
		bIsDragging = true;
	}
}

void UTopBarWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bIsDragging = false;
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
}
