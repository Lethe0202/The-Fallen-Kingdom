// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "QuestManager.generated.h"

class AQuestLocationVolume;
class UQuestInfo;
class UQuestTask;
class UQuestObjective;

USTRUCT()
struct FQuestAcceptInfo
{
	GENERATED_BODY()

public:
	FText QuestName;
	FText QuestDetail;
	TArray<FText> QuestObjective;
	TArray<bool> bQuestObjectiveComplete;
};

class UQuestGiverComponent;

UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UQuestManager : public UObject
{
	GENERATED_BODY()
	
public:
	void LoadNPCQuestFromCurrentWorld();

	//플레이어가 초기화 된 이후 호춯
	void InitQuestDelegate();
	
	//Actor가 퀘스트 목적을 달성했을때 Notify
	UFUNCTION(BlueprintCallable)
	void NotifyQuestQuantity(AActor* Actor, const FName& QuantityID, int32 Quantity);

	//퀘스트 컨디션(목적)완료
	void QuestConditionCompleted(const FName& QuestID);

	//성공한 퀘스트 QuestID
	UFUNCTION()
	void HandleQuestSuccess(const FName& QuestID);

	//퀘스트 수락 후, 진행중으로 변경된 QuestID
	UFUNCTION()
	void UpdateInProgressQuest(const FName& QuestID);
	
	void UpdateQuestState();
	
	FQuestAcceptInfo GetQuestAcceptInfo(const FName& QuestID);
	const TArray<FQuestReward> GetRewardItemID(const FName& QuestID);
	
	//퀘스트 위치볼륨 활성화
	void ActiveQuestVolumeObjective(const FName& VolumeID, const FVector Location, float Radius);

	//모든 퀘스트 목적오브젝트 활/비활성화 업데이트
	void DeActivateQuestObjective(const FName& QuestObjectiveID);

	//퀘스트 위치볼륨 비활성화
	void DeActiveQuestVolumeObjective(const FName& VolumeID);

	//퀘스트 생성
	UQuestInfo* CreateQuestInfo(const FName& QuestID);

	//퀘스트 Task생성
	TArray<TObjectPtr<UQuestTask>> CreateQuestTaskInfo(const FName& QuestID);

private:
	//퀘스트 Task의 Objectives타입에 맞게 초기화 후 반환
	UQuestTask* CreateQuestTask(const FQuestTaskRow& TaskRow);

public:
	const TArray<UQuestGiverComponent*>& GetWorldAllQuestGiverComponents() const;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> QuestInfoDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> QuestTaskDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AQuestLocationVolume> LocationVolumeActorClass;
	
private:	
	UPROPERTY()
	TArray<UQuestGiverComponent*> CurWorldQuestGivers;
	
	UPROPERTY()
	TMap<FName, AQuestLocationVolume*> QuestVolumeObjectiveMap;
	
	UPROPERTY()
	TMap<FName, AActor*> QuestStaticObjectiveMap;
};

