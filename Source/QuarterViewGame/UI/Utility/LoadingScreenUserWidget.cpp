// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreenUserWidget.h"

#include "Animation/WidgetAnimation.h"

void ULoadingScreenUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULoadingScreenUserWidget::RemoveWidget()
{
	if (UGameViewportClient* GameViewportClient = GetGameInstance()->GetGameViewportClient())
	{
		GameViewportClient->RemoveViewportWidgetContent(this->TakeWidget());
		this->RemoveFromParent();
	}
}

void ULoadingScreenUserWidget::PlayAnim_Implementation()
{
	PlayAnimation(WidgetAnimation, 0, 1);
}
