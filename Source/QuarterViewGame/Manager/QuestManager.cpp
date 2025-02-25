// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManager.h"

#include "AIBossCharacter.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "../Quest/QuestTask.h"
#include "../Quest/QuestGiverComponent.h"
#include "../Quest/QuestLogComponent.h"
#include "../Quest/QuestLocationVolume.h"

#include "EngineUtils.h"
#include "PlayerCharacter.h"

void UQuestManager::LoadNPCQuestFromCurrentWorld()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
	
	CurWorldQuestGivers.Empty();
	
    for (TActorIterator<AActor> It(World); It; ++It)
    {
        AActor* Actor = *It;
        if (!Actor)
        {
            continue;
        }
    	
        if (UQuestGiverComponent* QuestGiver = Actor->FindComponentByClass<UQuestGiverComponent>())
        {
            CurWorldQuestGivers.Add(QuestGiver);
        }
    }
}

void UQuestManager::InitQuestDelegate()
{
	for (auto& QuestGiverComponent : CurWorldQuestGivers)
	{
		APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		UQuestLogComponent* QuestLogComponent = PlayerPawn->GetComponentByClass<UQuestLogComponent>();
		if (QuestLogComponent)
		{
			//QuestPanelWidget에서 수락 버튼 눌렀을때 실행되는 델리게이트에 함수 등록
			QuestLogComponent->OnAddQuest.AddDynamic(QuestGiverComponent, &UQuestGiverComponent::OnInteractorAddQuest);

			//InProgress상태에서 상호작용은 포기 버튼을 활성화시킨다. QuestPanelWidget에서 포기 버튼 눌렀을때 실행되는 델리게이트에 함수 등록
			QuestLogComponent->OnRemoveQuest.AddDynamic(QuestGiverComponent, &UQuestGiverComponent::OnInteractorRemoveQuest);
		}
	}
}

void UQuestManager::NotifyQuestQuantity(AActor* Actor, const FName& QuantityID, int32 Quantity)
{
	APlayerCharacter* Character = Cast<APlayerCharacter>(Actor);
	if (Character)
	{
		Character->OnWorkObjectiveID.Broadcast(QuantityID);
	}
}

void UQuestManager::QuestConditionCompleted(const FName& QuestID)
{
    for (UQuestGiverComponent* QuestGiver : CurWorldQuestGivers)
    {
        if (QuestGiver->IsProgressQuest(QuestID))
        {
            QuestGiver->QuestConditionCompleted(QuestID);
        }
    }
}

void UQuestManager::HandleQuestSuccess(const FName& QuestID)
{
	for (UQuestGiverComponent* QuestGiver : CurWorldQuestGivers)
	{
		QuestGiver->UpdateSuccessfulQuest(QuestID);
	}
}

void UQuestManager::UpdateInProgressQuest(const FName& QuestID)
{
	for (UQuestGiverComponent* QuestGiver : CurWorldQuestGivers)
	{
		QuestGiver->UpdateInProgressQuest(QuestID);
	}
}

void UQuestManager::UpdateQuestState()
{
	for (UQuestGiverComponent* QuestGiver : CurWorldQuestGivers)
	{
		QuestGiver->UpdateQuestState();
	}
}

UQuestInfo* UQuestManager::CreateQuestInfo(const FName& QuestID)
{
	const FQuestInfoRow* QuestInfoRow = QuestInfoDataTable->FindRow<FQuestInfoRow>(QuestID, TEXT(""));
	
	if (QuestInfoRow != nullptr)
	{
		UQuestInfo* QuestInfo = NewObject<UQuestInfo>(this);
		QuestInfo->Init(QuestID, *QuestInfoRow);
		
		return QuestInfo;
	}

	return nullptr;
}

TArray<TObjectPtr<UQuestTask>> UQuestManager::CreateQuestTaskInfo(const FName& QuestID)
{
	const FQuestInfoRow* QuestInfoRow = QuestInfoDataTable->FindRow<FQuestInfoRow>(QuestID, TEXT(""));

	TArray<TObjectPtr<UQuestTask>> QuestTaskList;

	for (const FName& QuestTaskID : QuestInfoRow->TaskInfoIDArray)
	{
		const FQuestTaskRow* QuestTaskInfoRow = QuestTaskDataTable->FindRow<FQuestTaskRow>(QuestTaskID, TEXT(""));
		
		if (QuestTaskInfoRow != nullptr)
		{
            QuestTaskList.Add(CreateQuestTask(*QuestTaskInfoRow));		
		}
	}
	
	return QuestTaskList;
}

UQuestTask* UQuestManager::CreateQuestTask(const FQuestTaskRow& TaskRow)
{
    UQuestTask* QuestTask = NewObject<UQuestTask>(this);
	QuestTask->InitTask(TaskRow);

    return QuestTask;
}

const TArray<FQuestReward> UQuestManager::GetRewardItemID(const FName& QuestID)
{
	const FQuestInfoRow* QuestInfoRow = QuestInfoDataTable->FindRow<FQuestInfoRow>(QuestID, TEXT(""));
	
	if (QuestInfoRow != nullptr)
	{
		return QuestInfoRow->QuestRewardArray;
	}

	return TArray<FQuestReward>();
}

FQuestAcceptInfo UQuestManager::GetQuestAcceptInfo(const FName& QuestID)
{
	FQuestAcceptInfo QuestAcceptInfo;
	FQuestInfoRow* QuestInfoRow = QuestInfoDataTable->FindRow<FQuestInfoRow>(QuestID, TEXT(""));
	
	if (QuestInfoRow != nullptr)
	{
		QuestAcceptInfo.QuestName = QuestInfoRow->QuestInfoDisplayName;
		QuestAcceptInfo.QuestDetail = QuestInfoRow->QuestDetail;

		FQuestTaskRow* QuestTaskInfoRow = QuestTaskDataTable->FindRow<FQuestTaskRow>( QuestInfoRow->TaskInfoIDArray[0], TEXT(""));
		
		if (QuestTaskInfoRow != nullptr)
		{
			UQuestTask* QuestTask = CreateQuestTask(*QuestTaskInfoRow);
			if (QuestTask != nullptr)
			{
				QuestAcceptInfo.QuestObjective = QuestTask->GetCurrentTaskState();
			}
		}
	}

	return QuestAcceptInfo;
}

void UQuestManager::ActiveQuestVolumeObjective(const FName& VolumeID, const FVector Location, float Radius)
{
	//ID Map에 저장
	AQuestLocationVolume* QuestLocationVolume = GetWorld()->SpawnActor<AQuestLocationVolume>(LocationVolumeActorClass, Location, FRotator(0, 0, 0));
	QuestLocationVolume->SetLocationVolumeID(VolumeID);
	QuestLocationVolume->SetActive(true);
	
	QuestVolumeObjectiveMap.Add(VolumeID, QuestLocationVolume);

	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UWorldMapManager* WorldMapManager = QuarterViewGameInstance->GetWorldMapManager())
		{
			WorldMapManager->AddNewMapIconItem(QuestLocationVolume);
		}
	}
}

void UQuestManager::DeActivateQuestObjective(const FName& QuestObjectiveID)
{
	//일단은 location만
	DeActiveQuestVolumeObjective(QuestObjectiveID);
}

void UQuestManager::DeActiveQuestVolumeObjective(const FName& VolumeID)
{
	if (QuestVolumeObjectiveMap.Contains(VolumeID))
	{
		int32 UniqueID = QuestVolumeObjectiveMap[VolumeID]->GetUniqueID();
		QuestVolumeObjectiveMap[VolumeID]->Destroy();
		QuestVolumeObjectiveMap.Remove(VolumeID);

		if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (UWorldMapManager* WorldMapManager = QuarterViewGameInstance->GetWorldMapManager())
			{
				WorldMapManager->RemoveMapIconItem(UniqueID);
			}
		}
	}
}

const TArray<UQuestGiverComponent*>& UQuestManager::GetWorldAllQuestGiverComponents() const
{
	return CurWorldQuestGivers;
}


