// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScrollableWidget.generated.h"

class USizeBox;
class UOverlay;
class UCanvasPanel;

/**
 * 상속받는 요소를 마우스로 스크롤하여 사용할 수 있음
 * 현재 미니맵에 사용중
 */
UCLASS()
class QUARTERVIEWGAME_API UScrollableWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove( const FGeometry& InGeometry, const FPointerEvent& InMouseEvent ) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	//최대 최소 확대를 조정하고, 화면밖은 드래그 및 확대 되지않도록 화면 재조정
	void RefreshSizeBox();
	
protected:
	bool bIsDragging = false;
	FVector2D DragOffset;
	
	UPROPERTY(EditDefaultsOnly ,BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USizeBox> ScrollableSizeBox;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> ScrollableOverlayPanel;
	
	//0 ~ 1 0 : 최소, 1 : 최대
	float CurScale = 0;
	float MinWidgetSize = 1000.f;
	float MaxWidgetSize = 2000.f;
};
