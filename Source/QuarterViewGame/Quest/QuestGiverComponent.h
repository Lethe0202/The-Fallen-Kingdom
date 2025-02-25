// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "QuarterViewGame/Interface/SaveableInterface.h"
#include "QuestGiverComponent.generated.h"

class UQuestStateWidget;

UENUM(BlueprintType)
enum class EQuestState
{
	Lock,
	Waiting,
	InProgress,
	CompletionWaiting, 
	Success,
};

class UQuestPanelWidget;
class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UQuestGiverComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UQuestGiverComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//누군가 상호작용 했을 시 호출
	UFUNCTION(BlueprintCallable)
	void QuestInterct(AActor* Interactor);
	
	//퀘스트 Task의 목표를 다 완료 했을때 호출  --> 퀘스트 완료 대기 상태
	void QuestConditionCompleted(const FName& QuestID);
	
	//완료된 퀘스트 ID를 받아서 퀘스트관련 상태 업데이트
	void UpdateSuccessfulQuest(const FName& QuestID);
	
	//InProgress 퀘스트 업데이트 -> Save/Load 위함
	void UpdateInProgressQuest(const FName& QuestID);
	
	void UpdateQuestState();

	UFUNCTION()
	void OnInteractorAddQuest(const FName& QuestID, const FText& QuestDisplayName);

	UFUNCTION()
	void OnInteractorRemoveQuest(const FName& QuestID);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void InitQuestMap();
public:
	const bool IsProgressQuest(const FName& QuestID) const;
	const TArray<FName> GetAllQuestIDArray() const;
	TSoftObjectPtr<UTexture2D> GetCurrentStateIcon() const;
	FColor GetCurrentStateIconColor() const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestPanelWidget> QuestPanelWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "QuestGiver", meta = (AllowPrivateAccess = "true"))
	TArray<FDataTableRowHandle> QuestInfoRowHandleArray;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestGiver | UI",  meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestStateWidget> QuestStateWidgetClass;
	
	TMap<EQuestState, TArray<FName>> QuestState;
	
/*
 * 잠겨있는 퀘스트들의 상태를 업데이트 해야됨
 * 모든 잠겨있는 퀘스트를 순회하면 성능에 좋지않기때문에
 * 잠겨있는 퀘스트와, 선행퀘스트를 캐싱하여 관리함.
 */
	//PreQuestCache에서 퀘스트가 완료되었을때 Lock QuestState를 업데이트해야하는 퀘스트 목록을 캐싱한다.
	//PreQuestName, LockQuestName(잠겨있는 퀘스트)
	TMap<FName, TArray<FName>> PreQuestCache;

	//캐싱된 PreQuestCache에서 LockQuestName을 찾은 후, PreQuestNames(TArray<FName>)을 하나씩 지운다. PreQuestNames가 Empty면 Lock상태가 풀리게 된다.
	//TMap<LockQuestName(잠겨있는 퀘스트), PreQuestNames(잠겨있는 퀘스트의 선행 퀘스트 리스트)>
	TMap<FName, TArray<FName>> PreQuestForLockedQuestMap;
	
	UQuestPanelWidget* QuestPanelWidget;
	UWidgetComponent* WidgetComponent;
};


