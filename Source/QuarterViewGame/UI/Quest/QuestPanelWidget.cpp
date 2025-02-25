// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestPanelWidget.h"

#include "QuestObjectiveTextBlock.h"
#include "../Quest/QuestLogComponent.h"
#include "../Quest/QuestTask.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/QuestManager.h"

#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"

void UQuestPanelWidget::InitQuestPanel_Implementation(const FName& QuestID, AActor* Interactor, EQuestPanelType QuestPanelType)
{
	if (CurQuestID != QuestID)
	{
		ClearPanel();
		CurQuestID = QuestID;
	}
	
	//블루프린트에서 Reward UI와 상태 버튼 초기화
	if (QuestPanelType == EQuestPanelType::Accept)
	{
		InitAccept(QuestID);
	}
	else if (QuestPanelType == EQuestPanelType::InProgress)
	{
		InitInProgress(QuestID, Interactor);
	}
	else if (QuestPanelType == EQuestPanelType::Completion)
	{
		InitCompletion(QuestID);
	}
}

void UQuestPanelWidget::ClearPanel()
{
	Reward_Horizontal->ClearChildren();
	ButtonAction_Horizontal->ClearChildren();
	ObjectiveText_Vertical->ClearChildren();
}

void UQuestPanelWidget::InitInProgress(const FName& QuestID, AActor* Interactor)
{
	if (UQuestLogComponent* QuestLogComponent = Interactor->GetComponentByClass<UQuestLogComponent>())
	{
		const UQuestInfo* QuestInfo = QuestLogComponent->GetQuestInfo(QuestID);
		if (QuestInfo != nullptr)
		{
			QuestName_Text->SetText(QuestInfo->GetQuestDisplayName());
			QuestDetail_Text->SetText(QuestInfo->GetQuestDetailDisplayName()); 
			
			const TArray<FText> TaskStateArray = QuestInfo->GetTaskStateArray();
			if (!TaskStateArray.IsEmpty())
			{
				FString TaskStateString;
				
				ObjectiveText_Vertical->ClearChildren();
				
				for (int i = 0; i < TaskStateArray.Num(); ++i)
				{
					UQuestObjectiveTextBlock* QuestObjectiveTextBlock = CreateWidget<UQuestObjectiveTextBlock>(this, ObjectiveTextWidgetClass);
					
					QuestObjectiveTextBlock->InitObjectiveTextBlock(TaskStateArray[i], false);
					ObjectiveText_Vertical->AddChild(QuestObjectiveTextBlock);
				}
			}
		}
	}
}

void UQuestPanelWidget::InitAccept(const FName& QuestID)
{
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UQuestManager* QuestManager = QuarterViewGameInstance->GetQuestManager())
		{
			FQuestAcceptInfo QuestAcceptInfo = QuestManager->GetQuestAcceptInfo(QuestID);

			QuestName_Text->SetText(QuestAcceptInfo.QuestName);
			QuestDetail_Text->SetText(QuestAcceptInfo.QuestDetail);
			
			for (int i = 0; i < QuestAcceptInfo.QuestObjective.Num(); ++i)
			{
				UQuestObjectiveTextBlock* QuestObjectiveTextBlock = CreateWidget<UQuestObjectiveTextBlock>(this , ObjectiveTextWidgetClass);
				
				QuestObjectiveTextBlock->InitObjectiveTextBlock( QuestAcceptInfo.QuestObjective[i], false);
				ObjectiveText_Vertical->AddChild(QuestObjectiveTextBlock);
			}
		}
	}
}

void UQuestPanelWidget::InitCompletion(const FName& QuestID)
{
}