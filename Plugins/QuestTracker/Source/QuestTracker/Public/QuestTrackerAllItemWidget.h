// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTrackerAllItemWidget.generated.h"

class UButton;
class UTextBlock;
class UQuestTrackerData;
/**
 * 
 */
UCLASS()
class QUESTTRACKER_API UQuestTrackerAllItemWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnTrackerButtonClicked();
	
//public:
	//void SetQuestTrackerData(const UQuestTrackerData* QuestTrackerData);

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelName_Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TrackingButton;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ActorName_Text;

	FString LevelPath;
	FVector TargetLocation;
};
