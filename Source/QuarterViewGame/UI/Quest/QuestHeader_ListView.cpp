// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestHeader_ListView.h"
#include "QuestHeaderEntryWidget.h"
#include "QuestTabWidget.h"
#include "Components/Button.h"

UQuestHeader_ListView::UQuestHeader_ListView(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UUserWidget& UQuestHeader_ListView::OnGenerateEntryWidgetInternal(UObject* Item, TSubclassOf<UUserWidget> DesiredEntryClass, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (IsDesignTime())
	{
		UQuestHeaderEntryWidget& EntryWidget = GenerateTypedEntry<UQuestHeaderEntryWidget>(DesiredEntryClass.Get(), OwnerTable);
		
		return EntryWidget;
	}
	else
	{
		UQuestHeaderEntryWidget& EntryWidget = GenerateTypedEntry<UQuestHeaderEntryWidget>(DesiredEntryClass.Get(), OwnerTable);
		
		return EntryWidget;
	}
}

void UQuestHeader_ListView::UpdateQuestDetailView(const FName& QuestID)
{
	OnQuestHeaderClicked.Broadcast(QuestID);
}