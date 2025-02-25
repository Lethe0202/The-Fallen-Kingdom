// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTask.h"

#include "ShaderPrintParameters.h"
#include "UIManager.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/QuestManager.h"
#include "../Manager/SaveManager.h"

void UQuestInfo::Init(const FName& InQuestID, const FQuestInfoRow& QuestInfoRow)
{
	QuestID = InQuestID;
	QuestDisplayName = QuestInfoRow.QuestInfoDisplayName;
	QuestDetailDisplay = QuestInfoRow.QuestDetail;
	
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInstance != nullptr)
	{
		UQuestManager* QuestManager = GameInstance->GetQuestManager();
		
		if (QuestManager != nullptr)
		{
			TArray<TObjectPtr<UQuestTask>> NewQuestTaskArray = QuestManager->CreateQuestTaskInfo(InQuestID);
			QuestTaskArray = NewQuestTaskArray;
		}
	}
}

void UQuestInfo::LoadQuestInfo(FInProgressQuestSaveData& QuestInfoSaveData)
{
	CurrentTaskIndex = QuestInfoSaveData.CurrentTaskIndex;

	for (int i = 0; i < QuestTaskArray[CurrentTaskIndex]->TaskObjectiveArray.Num(); ++i)
	{
		QuestTaskArray[CurrentTaskIndex]->TaskObjectiveArray[i]->bIsCompleted = QuestInfoSaveData.bTaskObjectiveCompleted[i];	
		QuestTaskArray[CurrentTaskIndex]->TaskObjectiveArray[i]->CurrentQuantity = QuestInfoSaveData.TaskObjectiveQuantity[i];
	}
	
	if (QuestTaskArray[CurrentTaskIndex]->CheckCompletion())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnWorkedQuest Completed"));
		
		OnUpdateQuestTaskCondition.Broadcast(QuestID);
		OnQuestConditionComplete.Broadcast(QuestID);
		
		if (QuestTaskArray.Num() - 1 <= CurrentTaskIndex)
		{
			//퀘스트 성공
			bCompletion = true;
		}
		else
		{
			//다음 태스크로 업데이트
			++CurrentTaskIndex;
		}
	}
	else
	{
		OnUpdateQuestTaskCondition.Broadcast(QuestID);
	}
}

void UQuestInfo::OnWorkedQuest(const FName& ObjectiveID)
{
	if (!IsValid(this))
	{
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("OnWorkedQuest %s"), *ObjectiveID.ToString());

	if (!QuestTaskArray.IsValidIndex(CurrentTaskIndex))
	{
		return;
	}
	
	QuestTaskArray[CurrentTaskIndex]->UpdateTask(ObjectiveID);
	OnUpdateQuestObjectiveCondition.Broadcast(ObjectiveID);
	
	if (QuestTaskArray[CurrentTaskIndex]->CheckCompletion())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnWorkedQuest Completed"));
		
		OnUpdateQuestTaskCondition.Broadcast(QuestID);
		OnQuestConditionComplete.Broadcast(QuestID);
		
		if (QuestTaskArray.Num() - 1 <= CurrentTaskIndex)
		{
			//퀘스트 성공
			bCompletion = true;
		}
		else
		{
			//다음 태스크로 업데이트
			++CurrentTaskIndex;
		}
	}
	else
	{
		OnUpdateQuestTaskCondition.Broadcast(QuestID);
	}
}

const TArray<FText> UQuestInfo::GetTaskStateArray() const
{
	return QuestTaskArray[CurrentTaskIndex]->GetCurrentTaskState();
}

void UQuestInfo::SetActiveQuest(bool bActive, UWorld* World)
{
	if (QuestTaskArray.IsValidIndex(CurrentTaskIndex))
	{
		QuestTaskArray[CurrentTaskIndex]->SetActiveTask(bActive, World);	
	}
}

void UQuestTask::InitTask(const FQuestTaskRow& TaskRow)
{
	TaskDescription = TaskRow.QuestTaskisplayName;

	for (int i = 0; i < TaskRow.TaskObjectives.Num(); ++i)
	{
		UQuestObjective* QuestObjective = nullptr;
	
		switch (TaskRow.TaskObjectives[i].QuestType)
		{
		case EQuestType::Kill:
			QuestObjective = NewObject<UQuestObjective_Kill>(this);
			break;
				
		case EQuestType::Location:
			QuestObjective = NewObject<UQuestObjective_Location>(this);
			break;
				
		case EQuestType::Interact:
			QuestObjective = NewObject<UQuestObjective_Interact>(this);
			break;
				
		default:
			UE_LOG(LogTemp, Warning, TEXT("Unknown QuestType: %d"), (int32)TaskRow.TaskObjectives[i].QuestType);
			break;
		}

		TaskObjectiveArray.Add(QuestObjective);
		QuestObjective->InitObjective(TaskRow.TaskObjectives[i]);
	}
}

void UQuestTask::UpdateTask(const FName& ObjectiveID)
{
	for (auto Objective : TaskObjectiveArray)
	{
		if (Objective != nullptr)
		{
			Objective->UpdateObjective(ObjectiveID);	
		}
	}
}

void UQuestTask::SetActiveTask(bool bActive, UWorld* World)
{
	for (auto& Objective : TaskObjectiveArray)
	{
		Objective->SetActiveObjective(bActive, World);
	}
}

bool UQuestTask::CheckCompletion()
{
	for (auto Objective : TaskObjectiveArray)
	{
		if (Objective->CheckCompletion() == false)
		{
			return false;
		}
	}
	
	return true;
}

const TArray<FText> UQuestTask::GetCurrentTaskState() const
{
	TArray<FText> TaskDescriptionState;
	
	for (int i = 0; i < TaskObjectiveArray.Num(); ++i)
	{
		TaskDescriptionState.Add(TaskObjectiveArray[i]->GetCurrentObjectiveState());
	}
	
	return TaskDescriptionState;
}

void UQuestObjective::QuestObjectiveConditionUpdate()
{
	if (GetWorld())
	{
		UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
		if (UUIManager* UIManager = GameInstance->GetUIManager())
		{
			UIManager->QuestObjectiveConditionUpdate(GetCurrentObjectiveState());
		}
	}
}

void UQuestObjective::QuestObjectiveComplete()
{
	if (GetWorld())
	{
		UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
		if (UUIManager* UIManager = GameInstance->GetUIManager())
		{
			UIManager->QuestObjectiveComplete(TaskDescription);
		}
	}
}
