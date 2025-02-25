// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseSlotWidget.generated.h"

class UImage;

UCLASS()
class QUARTERVIEWGAME_API UBaseSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* IconImage;
	
	FName SlotItemID;
};
