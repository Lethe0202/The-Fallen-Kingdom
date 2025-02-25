// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ListView.h"
#include "QuestHeader_ListView.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestHeaderClicked, const FName&, QuestID);

UCLASS()
class QUARTERVIEWGAME_API UQuestHeader_ListView : public UListView
{
	GENERATED_BODY()
public:
	UQuestHeader_ListView(const FObjectInitializer& ObjectInitializer);

	void UpdateQuestDetailView(const FName& QuestID);
	
protected:
	virtual UUserWidget& OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable) override;
	
public:
	FOnQuestHeaderClicked OnQuestHeaderClicked;
};
