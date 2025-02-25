// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestObjectiveTextBlock.generated.h"

class UTextBlock;

/**
 *  
 */
UCLASS()
class QUARTERVIEWGAME_API UQuestObjectiveTextBlock : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitObjectiveTextBlock(const FText& Text, bool bSuccess);

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextBlock;
};
