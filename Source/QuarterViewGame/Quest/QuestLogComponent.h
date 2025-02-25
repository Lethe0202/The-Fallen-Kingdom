// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarterViewGame/Interface/SaveableInterface.h"
//#include "QuestTask.h"
#include "QuestLogComponent.generated.h"

class UQuestInfo;

UENUM(BlueprintType)
enum class EQuestType
{
	Location,
	CutScene,
	Interact,
	Kill,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddQuest, const FName&, QuestID, const FText&, QuestDisplayName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRemoveQuest, const FName&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSuccessQuest, const FName&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCondition, const FName&, QuestID);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UQuestLogComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()
	
public:
	UQuestLogComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddQuest(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	void RemoveQuest(const FName& QuestID);
	
	UFUNCTION(BlueprintCallable)
	void SuccessQuest(const FName& QuestID);
	
	const UQuestInfo* GetQuestInfo(const FName& QuestID);
	
	UFUNCTION(BlueprintCallable)
	const bool IsCompletionQuest(const FName& QuestID);

	UFUNCTION(BlueprintCallable)
	const bool IsActivateQuest(const FName& QuestID);

	const TArray<TObjectPtr<UQuestInfo>> GetProgressQuest() const;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleConditionComplete(const FName& QuestID);

	UFUNCTION()
	void HandleUpdateQuestTaskCondition(const FName& QuestID);
	
	UFUNCTION()
	void HandleUpdateQuestObjectiveCondition(const FName& ObjectiveID);
	
public:
	/* ISaveableInterface */
	virtual void LoadData(USaveGameDataBase& LoadData) override;
	virtual void SaveData(/*out*/USaveGameDataBase*& SaveData) override;
	virtual ESaveDataType SaveDataType() override;
	
public:
	FOnAddQuest OnAddQuest;
	FOnRemoveQuest OnRemoveQuest;
	FOnSuccessQuest OnSuccessQuest;
	FOnUpdateCondition OnUpdateCondition;
	
private:
	UPROPERTY()
	TMap<FName, TObjectPtr<UQuestInfo>> InProgressQuest;
	
	UPROPERTY()
	TArray<FName> SuccessQuestArray;

	bool bLoadPhase = false;
};


