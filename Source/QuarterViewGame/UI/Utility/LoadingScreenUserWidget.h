// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingScreenUserWidget.generated.h"

/**
 * 레벨 전환 시 보여지는 로딩스크린
 */
UCLASS()
class QUARTERVIEWGAME_API ULoadingScreenUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void PlayAnim();

protected:
	UFUNCTION(BlueprintCallable)
	void RemoveWidget();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidgetAnim), Transient)
	TObjectPtr<UWidgetAnimation> WidgetAnimation;
};
