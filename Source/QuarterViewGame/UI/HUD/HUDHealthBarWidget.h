// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDHealthBarWidget.generated.h"

class UCharacterStatusComponent;
class UProgressBar;

UCLASS()
class QUARTERVIEWGAME_API UHUDHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init(UCharacterStatusComponent* CharacterStatusComponent);

	virtual void NativeInit(UCharacterStatusComponent* CharacterStatusComponent);
	
protected:
	virtual void InitWidget();
	
protected:
	UFUNCTION()
	void OnHealthChange(float NewPercent);
	
protected:
	virtual void UpdateHealthBar(float NewPercent);

protected:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;
};
