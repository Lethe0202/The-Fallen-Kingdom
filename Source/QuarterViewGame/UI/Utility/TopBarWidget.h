// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopBarWidget.generated.h"

/**
 * 
 */

class UCanvasPanelSlot;

UCLASS()
class QUARTERVIEWGAME_API UTopBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	UFUNCTION(BlueprintCallable)
	void SetTargetWidget(class UUserWidget* InTargetWidget) { TargetWidget = InTargetWidget; }

protected:
	UPROPERTY()
	TObjectPtr<UUserWidget> TargetWidget;

	TObjectPtr<UCanvasPanelSlot> CanvasSlot;
private:
	bool bIsDragging = false;
	FVector2D DragOffset;

	FVector2D ViewportSize;
	FVector2d WidgetSize;
};
