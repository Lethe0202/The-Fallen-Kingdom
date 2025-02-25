// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilityProgressBarWidget.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
class UPlayerAbilityComponent;


UCLASS()
class QUARTERVIEWGAME_API UAbilityProgressBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UPlayerAbilityComponent* AbilityComponent);

protected:
	UFUNCTION()
	void OnUpdateProgressBar(float Percent, float HoldTime, float MinPerfectZoneTime, float MaxPerfectZoneTime);

private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* AbilityProgressBar;

	UPROPERTY(meta = (BindWidget))
	UImage* AbilityPerfectZoneImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HoldTimeTextBlock;
};
