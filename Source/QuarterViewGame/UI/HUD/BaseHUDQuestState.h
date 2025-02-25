// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseHUDQuestState.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UBaseHUDQuestState : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDescription(FText InDescription);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Description_TextBlock;
};
