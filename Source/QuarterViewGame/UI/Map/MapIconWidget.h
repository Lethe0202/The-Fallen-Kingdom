// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapIconWidget.generated.h"

/**
 * 
 */

class UImage;

UCLASS()
class QUARTERVIEWGAME_API UMapIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetMapIcon(const TSoftObjectPtr<UTexture2D>& MapIconTexture);
	void SetMapColorAndOpacity(const FColor& InColorAndOpacity);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;
	
private:
	float OrthoWidth;
};
