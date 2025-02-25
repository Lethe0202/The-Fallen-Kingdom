// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "QuestTrackerItemEntry.generated.h"

/**
 * 
 */

class UButton;
class UTextBlock;

UCLASS()
class QUESTTRACKER_API UQuestTrackerItemEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnEntryReleased() override;
	
	UFUNCTION()
	void OnTrackingButtonClicked();
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestID_Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDisplayName_Text;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelName_Text;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TrackingButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ActorName_Text;

	UPROPERTY()
	UQuestTrackerData* QuestTrackerData;
};
