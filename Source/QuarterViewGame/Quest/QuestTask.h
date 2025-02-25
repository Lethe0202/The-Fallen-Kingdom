// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuarterViewGameInstance.h"
#include "QuestManager.h"
#include "SaveManager.h"
#include "UIManager.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "QuestTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestConditionComplete, const FName&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateQuestTaskCondition, const FName&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateQuestObjectiveCondition, const FName&, ObjectiveID);

class UQuestManager;

UCLASS(Blueprintable)
class UQuestInfo : public UObject
{
    GENERATED_BODY()
    
public:
    void Init(const FName& InQuestID, const FQuestInfoRow& QuestInfoRow);
    
    void LoadQuestInfo(FInProgressQuestSaveData& QuestInfoSaveData);
    
    //퀘스트의 목적(목표)가 업데이트됐을때
    UFUNCTION()
    void OnWorkedQuest(const FName& ObjectiveID);
    
    const bool IsCompletion() { return bCompletion; }
    
public:
    FOnQuestConditionComplete OnQuestConditionComplete;
    FOnUpdateQuestTaskCondition OnUpdateQuestTaskCondition;
    FOnUpdateQuestObjectiveCondition OnUpdateQuestObjectiveCondition;
    
public:
    const FName& GetQuestID() const { return QuestID; }
    const FText& GetQuestDisplayName() const { return QuestDisplayName; }
    const FText& GetQuestDetailDisplayName() const { return QuestDetailDisplay; }
    const int32 GetCurrentTaskIndex() const { return CurrentTaskIndex;}
    const UQuestTask& GetCurrentTask() const { return *QuestTaskArray[CurrentTaskIndex]; }
    const TArray<FText> GetTaskStateArray() const;
    void SetActiveQuest(bool bActive, UWorld* World);
    
private:
    UPROPERTY()
    FName QuestID;

    UPROPERTY()
    FText QuestDisplayName;

    UPROPERTY()
    FText QuestDetailDisplay;
    
    UPROPERTY()
    TArray<TObjectPtr<UQuestTask>> QuestTaskArray;

    UPROPERTY()
    int32 CurrentTaskIndex;

    bool bCompletion = false;
};

UCLASS(Blueprintable)
class UQuestTask : public UObject
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TaskID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText TaskDescription;

    UPROPERTY()
    TArray<TObjectPtr<UQuestObjective>> TaskObjectiveArray;
    
    virtual void InitTask(const FQuestTaskRow& TaskRow);
    virtual void UpdateTask(const FName& ObjectiveID);
    virtual void SetActiveTask(bool bActive, UWorld* World);
    virtual bool CheckCompletion();
    virtual const TArray<FText> GetCurrentTaskState() const;
};

UCLASS(Blueprintable)
class UQuestObjective : public UObject
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText TaskDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsCompleted;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentQuantity = 0;
    
    virtual void InitObjective(const FTaskObjectives& TaskObjectiveInfo) {TaskDescription = TaskObjectiveInfo.Description;}
    virtual void UpdateObjective(const FName& ObjectiveID) {}
    virtual void SetActiveObjective(bool bActive, UWorld* World) {}
    virtual bool CheckCompletion() { return bIsCompleted; }
    virtual const FText GetCurrentObjectiveState() const { return TaskDescription; }
    int32 GetCurrentQuantity() const { return CurrentQuantity; }

protected:
    //UI에 목표가 업데이트됐다고 띄우기
    void QuestObjectiveConditionUpdate();

    //UI에 목표가 완료됨 띄우기
    void QuestObjectiveComplete();
    
};

UCLASS(Blueprintable)
class UQuestObjective_Kill : public UQuestObjective
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 KillQuantity;
    
    virtual void InitObjective(const FTaskObjectives& TaskObjectiveInfo) override
    {
        Super::InitObjective(TaskObjectiveInfo);
        TargetID = TaskObjectiveInfo.QuestTaskInfo_Kill.TargetID;
        KillQuantity = TaskObjectiveInfo.QuestTaskInfo_Kill.Quantity;
    } 
    
    virtual void UpdateObjective(const FName& ObjectiveID) override
    {
        if(TargetID == ObjectiveID)
        {
            ++CurrentQuantity;
            QuestObjectiveConditionUpdate();
        }
        
        if (CurrentQuantity >= KillQuantity)
        {
            bIsCompleted = true;
            QuestObjectiveComplete();
        }
    }
    
    virtual const FText GetCurrentObjectiveState() const override
    {
        const FString returnString = TaskDescription.ToString() + FString::FromInt(CurrentQuantity) + " / " + FString::FromInt(KillQuantity);
        
        return FText::FromString(returnString);
    }
};

UCLASS(Blueprintable)
class UQuestObjective_Location : public UQuestObjective
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName LocationID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ObjectiveLocation;
    
    virtual void InitObjective(const FTaskObjectives& TaskObjectiveInfo) override
    {
        Super::InitObjective(TaskObjectiveInfo);
        LocationID = TaskObjectiveInfo.QuestTaskInfo_Location.LocationTriggerID;
        ObjectiveLocation = TaskObjectiveInfo.QuestTaskInfo_Location.ObjectiveLocation;
    }
    
    virtual void SetActiveObjective(bool bActive, UWorld* World) override
    {
        UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(World->GetGameInstance());
        if (!QuarterViewGameInstance)
        {
            return;
        }
        
        UQuestManager* QuestManager = Cast<UQuestManager>(QuarterViewGameInstance->GetQuestManager());
        if (!QuestManager)
        {
            return;
        }
        
        if (bActive)
        {
            QuestManager->ActiveQuestVolumeObjective(LocationID, ObjectiveLocation, 0.f);    
        }
        else
        {
            QuestManager->DeActiveQuestVolumeObjective(LocationID);
        }
    }
    
    virtual void UpdateObjective(const FName& ObjectiveID) override
    {
        if (LocationID == ObjectiveID)
        {
            bIsCompleted = true;
            QuestObjectiveComplete();
        }
    }
};

UCLASS(Blueprintable)
class UQuestObjective_Interact : public UQuestObjective
{
    GENERATED_BODY()
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName InteractTargetID;
    
    virtual void UpdateObjective(const FName& ObjectiveID) override
    {
        bIsCompleted = true;
        QuestObjectiveComplete();
    }
};
