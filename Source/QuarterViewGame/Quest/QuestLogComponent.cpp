// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLogComponent.h"

#include "InventoryComponent.h"
#include "PlayerCharacter.h"
#include "QuestTask.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/QuestManager.h"
#include "../Manager/SaveManager.h"
#include "../Manager/UIManager.h"

// Sets default values for this component's properties
UQuestLogComponent::UQuestLogComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UQuestLogComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// ...
	
	if (UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UQuestManager* QuestManager = GameInstance->GetQuestManager())
		{
			OnSuccessQuest.AddDynamic(QuestManager, &UQuestManager::HandleQuestSuccess);
		}
	}
}

// Called every frame
void UQuestLogComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UQuestLogComponent::AddQuest(const FName& QuestID)
{
	if (InProgressQuest.Contains(QuestID))
	{
		return;
	}
	
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	UQuestInfo* QuestInfo = nullptr;
	
	if (GameInstance != nullptr)
	{
		UQuestManager* QuestManager = GameInstance->GetQuestManager();
		
		if (QuestManager != nullptr)
		{
			QuestInfo = QuestManager->CreateQuestInfo(QuestID);
			
			if (QuestInfo == nullptr)
			{
				return;
			}
			
			QuestInfo->PrefetchOuter(this);
			QuestInfo->SetActiveQuest(true, GetWorld());
		}
	}
	
	if (QuestInfo == nullptr)
	{
		return;
	}
	
	InProgressQuest.Add(QuestID, QuestInfo);
	
	if (APlayerCharacter* PlayerCharacter= Cast<APlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->OnWorkObjectiveID.AddDynamic(QuestInfo, &UQuestInfo::OnWorkedQuest);
		QuestInfo->OnQuestConditionComplete.AddDynamic(this, &UQuestLogComponent::HandleConditionComplete);
		QuestInfo->OnUpdateQuestTaskCondition.AddDynamic(this, &UQuestLogComponent::HandleUpdateQuestTaskCondition);
		QuestInfo->OnUpdateQuestObjectiveCondition.AddDynamic(this, &UQuestLogComponent::HandleUpdateQuestObjectiveCondition);
	}
	
	OnAddQuest.Broadcast(QuestID, QuestInfo->GetQuestDisplayName());
	
	if (!bLoadPhase && GameInstance)
	{
		GameInstance->GetUIManager()->QuestAccept(QuestInfo->GetQuestDisplayName());
		GameInstance->GetSaveManager()->SaveGameData();
	}
}

void UQuestLogComponent::RemoveQuest(const FName& QuestID)
{
	if (InProgressQuest.Contains(QuestID))
	{
		InProgressQuest[QuestID]->SetActiveQuest(false, GetWorld());
		InProgressQuest.Remove(QuestID);
		OnRemoveQuest.Broadcast(QuestID);
		
		UE_LOG(LogTemp, Warning, TEXT(" UQuestLogComponent::RemoveQuest"));
		
		UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance)
		{
			GameInstance->GetSaveManager()->SaveGameData();
		}
	}
}

void UQuestLogComponent::SuccessQuest(const FName& QuestID)
{
	if (InProgressQuest.Contains(QuestID))
	{
		FText QuestDisplayName = InProgressQuest[QuestID]->GetQuestDisplayName();
		
		InProgressQuest[QuestID]->MarkAsGarbage();
		
		InProgressQuest.Remove(QuestID);
		SuccessQuestArray.Add(QuestID);
		
		OnSuccessQuest.Broadcast(QuestID);
		OnRemoveQuest.Broadcast(QuestID);
		
		UE_LOG(LogTemp, Warning, TEXT(" UQuestLogComponent::SuccessQuest"));
		
		UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
		if (GameInstance)
		{
			GameInstance->GetUIManager()->QuestSuccess(QuestDisplayName);
			GameInstance->GetSaveManager()->SaveGameData();
		}
	}
}

const UQuestInfo* UQuestLogComponent::GetQuestInfo(const FName& QuestID)
{
	if (InProgressQuest.Contains(QuestID))
	{
		return InProgressQuest[QuestID];
	}
	else
	{
		return nullptr;
	}
}

//퀘스트 매니저 -> NPCArray -> 퀘스트 컨디션(목적)완료
void UQuestLogComponent::HandleConditionComplete(const FName& QuestID)
{
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInstance != nullptr)
	{
		if (UQuestManager* QuestManager = GameInstance->GetQuestManager())
		{
			QuestManager->QuestConditionCompleted(QuestID);
		}
	}
}

//퀘스트 Task 컨디션 업데이트
void UQuestLogComponent::HandleUpdateQuestTaskCondition(const FName& QuestID)
{
	OnUpdateCondition.Broadcast(QuestID);
}

void UQuestLogComponent::HandleUpdateQuestObjectiveCondition(const FName& ObjectiveID)
{
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInstance != nullptr)
	{
		UQuestManager* QuestManager = GameInstance->GetQuestManager();
		
		if (QuestManager != nullptr)
		{
			QuestManager->DeActivateQuestObjective(ObjectiveID);
		}
	}
}

const bool UQuestLogComponent::IsCompletionQuest(const FName& QuestID)
{
	if (InProgressQuest.Contains(QuestID))
	{
		return InProgressQuest[QuestID]->IsCompletion();
	}
	
	return false;
}

const bool UQuestLogComponent::IsActivateQuest(const FName& QuestID)
{
	if (InProgressQuest.Contains(QuestID) || SuccessQuestArray.Contains(QuestID))
	{
		return true;
	}
	
	return false;
}

const TArray<TObjectPtr<UQuestInfo>> UQuestLogComponent::GetProgressQuest() const
{
	TArray<TObjectPtr<UQuestInfo>> QuestInfos;

	InProgressQuest.GenerateValueArray(QuestInfos);
	
	return QuestInfos;
}

void UQuestLogComponent::LoadData(USaveGameDataBase& LoadData)
{
	bLoadPhase = true;
	
	UQuestInfoSaveData* QuestInfoSaveData = Cast<UQuestInfoSaveData>(&LoadData);
	SuccessQuestArray = QuestInfoSaveData->QuestSaveData.SuccessQuestArray;

	UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	UQuestManager* QuestManager = nullptr;

	if (QuarterViewGameInstance)
	{
		QuestManager = QuarterViewGameInstance->GetQuestManager();
	}
	
	if (QuestManager)
	{
		for (int i = 0; i < SuccessQuestArray.Num(); ++i)
		{
			QuestManager->HandleQuestSuccess(SuccessQuestArray[i]);
		}
	}
	
	TArray<FInProgressQuestSaveData> InProgressQuestSaveData = QuestInfoSaveData->QuestSaveData.InProgressQuestNameArray;
	for (int i = 0; i < InProgressQuestSaveData.Num(); i++)
	{
		AddQuest(InProgressQuestSaveData[i].QuestID);
		InProgressQuest[InProgressQuestSaveData[i].QuestID]->LoadQuestInfo(InProgressQuestSaveData[i]);
		QuestManager->UpdateInProgressQuest(InProgressQuestSaveData[i].QuestID);
	}
	
	if (QuestManager)
	{
		QuestManager->UpdateQuestState();
	}

	bLoadPhase = false;
}

void UQuestLogComponent::SaveData(USaveGameDataBase*& SaveData)
{
	SaveData = NewObject<UQuestInfoSaveData>();
	UQuestInfoSaveData* QuestInfoSaveData = Cast<UQuestInfoSaveData>(SaveData);
	QuestInfoSaveData->QuestSaveData.SuccessQuestArray = SuccessQuestArray;

	//진행중인 퀘스트의 정보를 추출해서 FInProgressQuestSaveData에 담음 
	for (auto& InProgressQuestInfo : InProgressQuest)
	{
		FInProgressQuestSaveData InProgressQuestSaveData;
		InProgressQuestSaveData.QuestID = InProgressQuestInfo.Key;
		InProgressQuestSaveData.bCompletion = InProgressQuestInfo.Value->IsCompletion();
		InProgressQuestSaveData.CurrentTaskIndex = InProgressQuestInfo.Value->GetCurrentTaskIndex();
		
		const UQuestTask& CurrentQuestTask = InProgressQuestInfo.Value->GetCurrentTask();
		TArray<UQuestObjective*> TaskObjectiveArray = CurrentQuestTask.TaskObjectiveArray;
		
		for (int i = 0; i < TaskObjectiveArray.Num(); i++)
		{
			InProgressQuestSaveData.bTaskObjectiveCompleted.Add(TaskObjectiveArray[i]->bIsCompleted);
			InProgressQuestSaveData.TaskObjectiveQuantity.Add(TaskObjectiveArray[i]->CurrentQuantity);
		}
		
		QuestInfoSaveData->QuestSaveData.InProgressQuestNameArray.Add(InProgressQuestSaveData);
	}
}

ESaveDataType UQuestLogComponent::SaveDataType()
{
	return ESaveDataType::Quest;
}