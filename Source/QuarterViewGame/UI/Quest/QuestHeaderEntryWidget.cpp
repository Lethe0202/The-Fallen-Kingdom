// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestHeaderEntryWidget.h"

#include "QuestHeader_ListView.h"
#include "QuestTabWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UQuestHeaderEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Button->OnClicked.AddDynamic(this, &UQuestHeaderEntryWidget::OnButtonClicked);
}

void UQuestHeaderEntryWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UQuestHeaderData* QuestHeaderInfo = Cast<UQuestHeaderData>(ListItemObject))
	{
		QuestID = QuestHeaderInfo->QuestID;
		QuestDisplayName->SetText(QuestHeaderInfo->QuestDisplayName); 
	}
	
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UQuestHeaderEntryWidget::OnButtonClicked()
{
	if (UQuestHeader_ListView* QuestHeader_ListView = Cast<UQuestHeader_ListView>(GetOwningListView()))
	{
		QuestHeader_ListView->UpdateQuestDetailView(QuestID);
	}
}
