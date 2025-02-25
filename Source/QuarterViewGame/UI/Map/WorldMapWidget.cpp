// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldMapWidget.h"
#include "QuarterViewGameInstance.h"
#include "../Manager/WorldMapManager.h"
#include "Map/MapIconWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"

void UWorldMapWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetGameInstance()))
	{
		if (UWorldMapManager* WorldMapManager = QuarterViewGameInstance->GetWorldMapManager())
		{
			WorldMapManager->OnWorldMapIconChanged.AddDynamic(this, &UWorldMapWidget::HandleUpdateIcon);
			
			if (UTexture2D* CurrentWorldMapTexture = WorldMapManager->GetCurrentWorldMapTexture())
			{
				WorldMap_Image->GetDynamicMaterial()->SetTextureParameterValue("WorldMapTexture", CurrentWorldMapTexture);
				
				OrthoWidth = WorldMapManager->GetCurrentWorldOrthoWidth();
				WorldMap_Image->GetDynamicMaterial()->SetScalarParameterValue("WorldOrthoWidth", OrthoWidth);
			}
			
			const TArray<FMapItem> MapItems = WorldMapManager->GetAllMapItems();
			
			for (const FMapItem& MapItem : MapItems)
			{
				UMapIconWidget* MapIconWidget = CreateWidget<UMapIconWidget>(this, IconWidgetClass);
				MapIconWidget->SetMapIcon(MapItem.Icon);
				MapIconWidget->SetMapColorAndOpacity(MapItem.Color);
				IconCanvasPanel->AddChildToCanvas(MapIconWidget);
				
				SetMapIconLocation(*MapIconWidget, MapItem.Location);
				
				if (MapItem.MapIconMovableType == EMapIconMovableType::Dynamic)
				{
					DynamicActorIDArray.Add(MapItem.OwnerUniqueID, MapItem);
				}
				
				IconWidgetMap.Add(MapItem.OwnerUniqueID, MapIconWidget);
			}
		}
	}
}

void UWorldMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!DynamicActorIDArray.IsEmpty())
	{
		for (auto& DynamicActorID : DynamicActorIDArray)
		{
			if (!DynamicActorID.Value.Owner.IsValid())
			{
				return;
			}
			
			const UMapIconWidget* MapIconWidget = IconWidgetMap[DynamicActorID.Key];
			FVector TargetLocation = DynamicActorID.Value.Owner->GetActorLocation();
			FVector2D TargetLocation2D = FVector2D(TargetLocation.X, TargetLocation.Y);
			SetMapIconLocation(*MapIconWidget, TargetLocation2D);
		}
	}
}

FReply UWorldMapWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply ReturnReply= Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetGameInstance()))
	{
		if (UWorldMapManager* WorldMapManager = QuarterViewGameInstance->GetWorldMapManager())
		{
			const TArray<FMapItem> MapItems = WorldMapManager->GetAllMapItems();
			for (const FMapItem& MapItem : MapItems)
			{
				UMapIconWidget* MapIconWidget = IconWidgetMap[MapItem.OwnerUniqueID];
				SetMapIconLocation(*MapIconWidget, MapItem.Location);
			}
		}
	}
	
	return ReturnReply;
}

void UWorldMapWidget::SetMapIconLocation(const UMapIconWidget& MapIconWidget, FVector2D Location)
{
	if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(MapIconWidget.Slot))
	{
		if (UCanvasPanelSlot* IconCanvasPanelSlot = Cast<UCanvasPanelSlot>(ScrollableSizeBox->Slot))
		{
			CanvasPanelSlot->SetAlignment(FVector2D(0.5, 0.5));
			CanvasPanelSlot->SetAnchors(FAnchors(0.5, 0.5));
				
			//카메라 직교 너비 --> 위치를 나눠줘야함
			//WorldLocation -> NormalizedLocation , 0 ~ 1;
			FVector2D NormalizedLocation = Location / OrthoWidth;
			
			//NormalizedLocation -> CanvasLocation
			//아이콘이 올라가는 캔버스 너비를 곱해줘서 CanvasLocation 구함, 캔버스의 사이즈의 X,Y는 동일하다는 가정하에 X로 계산함
			FVector2D CanvasLocation = NormalizedLocation * IconCanvasPanelSlot->GetSize().X;
				
			//최종 계산
			//좌표 방향과 XY를 캔버스 기준으로 맞춰줘야하기때문에 추가적인 계산
			CanvasPanelSlot->SetPosition(FVector2D(CanvasLocation.Y, CanvasLocation.X * -1));

			CanvasPanelSlot->SetAutoSize(true);
		}
	}
}

const float UWorldMapWidget::GetOrthoWidth() const
{
	return OrthoWidth;
}

void UWorldMapWidget::HandleUpdateIcon(int32 InUniqueID, const TSoftObjectPtr<UTexture2D>& Icon, const FColor& InColorAndOpacity)
{
	if (IconWidgetMap.Contains(InUniqueID))
	{
		IconWidgetMap[InUniqueID]->SetMapIcon(Icon);
		IconWidgetMap[InUniqueID]->SetMapColorAndOpacity(InColorAndOpacity);
	}
	else
	{
		UWorldMapManager* WorldMapManager = nullptr;
		
		if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetGameInstance()))
		{
			WorldMapManager = QuarterViewGameInstance->GetWorldMapManager();
		}
		
		if (!WorldMapManager)
		{
			return;
		}
		
		const FMapItem* MapItem = WorldMapManager->GetMapItem(InUniqueID);
		
		UMapIconWidget* MapIconWidget = CreateWidget<UMapIconWidget>(this, IconWidgetClass);
		MapIconWidget->SetMapIcon(MapItem->Icon);
		MapIconWidget->SetMapColorAndOpacity(MapItem->Color);
		IconCanvasPanel->AddChildToCanvas(MapIconWidget);

		SetMapIconLocation(*MapIconWidget, MapItem->Location);
		
		if (MapItem->MapIconMovableType == EMapIconMovableType::Dynamic)
		{
			DynamicActorIDArray.Add(MapItem->OwnerUniqueID, *MapItem);
		}
		
		IconWidgetMap.Add(MapItem->OwnerUniqueID, MapIconWidget);
	}
}
