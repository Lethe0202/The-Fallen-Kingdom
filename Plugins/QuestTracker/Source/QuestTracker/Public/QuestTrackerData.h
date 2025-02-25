// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestTrackerData.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnShowQuestTracker, const UQuestTrackerData*, QuestTrackerData);

UCLASS()
class QUESTTRACKER_API UQuestTrackerData : public UObject
{
	GENERATED_BODY()
	
public:
	FOnShowQuestTracker OnShowQuestTracker;
	
	FName QuestID;
	FName QuestDisplayName;
	
	TArray<FString> LevelPath;
	TArray<FString> LevelName;
	TArray<FString> ActorName;
	TArray<FVector> QuestOwnerActorWorldLocation;
};
