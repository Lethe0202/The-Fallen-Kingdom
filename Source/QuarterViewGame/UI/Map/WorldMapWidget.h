// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../UI/Utility/ScrollableWidget.h"
#include "WorldMapWidget.generated.h"

class UImage;
class UMapIconWidget;
class UTexture2D;

UCLASS()
class QUARTERVIEWGAME_API UWorldMapWidget : public UScrollableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void HandleUpdateIcon(int32 InUniqueID, const TSoftObjectPtr<UTexture2D>& Icon, const FColor& InColorAndOpacity);
	
protected:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	void SetMapIconLocation(const UMapIconWidget& MapIconWidget, FVector2D Location);
	
	UFUNCTION(BlueprintCallable)
	const float GetOrthoWidth() const;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> WorldMap_Image;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> IconCanvasPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMapIconWidget> IconWidgetClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<TObjectPtr<UMapIconWidget>> Icons;
	
	//UniqueID, MapIconWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<int32, TObjectPtr<UMapIconWidget>> IconWidgetMap;

	//UniqueID, MapItemData;
	UPROPERTY()
	TMap<int32, FMapItem> DynamicActorIDArray;
	
	float OrthoWidth;
};
