// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTabWidget.h"

#include "QuestHeaderEntryWidget.h"
#include "../Quest/QuestLogComponent.h"
#include "../Quest/QuestTask.h"
#include "QuestHeader_ListView.h"
#include "QuestPanelWidget.h"

void UQuestTabWidget::NativeConstruct()
{
	Super::NativeConstruct();

	QuestPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UQuestTabWidget::InitQuestTab()
{
	if (AController* PC = Cast<AController>(GetOwningPlayer()))
	{
		if (APawn* Pawn = Cast<APawn>(PC->GetPawn()))
		{
			if (UQuestLogComponent* QuestLogComponent = Pawn->GetComponentByClass<UQuestLogComponent>())
			{
				const TArray<UQuestInfo*> QuestInfoArray = QuestLogComponent->GetProgressQuest();
				
				for (UQuestInfo* QuestInfo : QuestInfoArray)
				{
					UQuestHeaderData* QuestHeaderInfo = NewObject<UQuestHeaderData>();
					QuestHeaderInfo->QuestDisplayName = QuestInfo->GetQuestDisplayName();
					QuestHeaderInfo->QuestID = QuestInfo->GetQuestID();
					QuestHeaderListView->AddItem(QuestHeaderInfo);
				}
				
				QuestHeaderListView->OnQuestHeaderClicked.AddDynamic(this, &UQuestTabWidget::OnUpdateQuestPanel);
				QuestLogComponent->OnAddQuest.AddDynamic(this, &UQuestTabWidget::OnAddQuestHeader);
				QuestLogComponent->OnRemoveQuest.AddDynamic(this, &UQuestTabWidget::OnRemoveQuestHeader);
				QuestLogComponent->OnUpdateCondition.AddDynamic(this, &UQuestTabWidget::OnUpdateQuestPanel);
			}
		}
	}
}

void UQuestTabWidget::OnAddQuestHeader(const FName& QuestID, const FText& QuestDisplayName)
{
	UQuestHeaderData* QuestHeaderInfo = NewObject<UQuestHeaderData>();
	QuestHeaderInfo->QuestDisplayName = QuestDisplayName;
	QuestHeaderInfo->QuestID = QuestID;
	QuestHeaderListView->AddItem(QuestHeaderInfo);
}

void UQuestTabWidget::OnRemoveQuestHeader(const FName& QuestID)
{
	TArray<UObject*> ListItem = QuestHeaderListView->GetListItems();
	for (int i = 0; i < ListItem.Num(); ++i)
	{
		if (Cast<UQuestHeaderData>(ListItem[i])->QuestID == QuestID)
		{
			QuestHeaderListView->RemoveItem(ListItem[i]);
			QuestPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UQuestTabWidget::OnUpdateQuestPanel(const FName& QuestID)
{
	QuestPanel->SetVisibility(ESlateVisibility::Visible);
	QuestPanel->InitQuestPanel(QuestID, GetOwningPlayer()->GetPawn(), EQuestPanelType::InProgress);
}
