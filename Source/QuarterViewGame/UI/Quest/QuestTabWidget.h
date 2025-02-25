// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestTabWidget.generated.h"

class UListView;

UCLASS(BlueprintType)
class UQuestHeaderData : public UObject
{
	GENERATED_BODY()
    
public:
	UPROPERTY(BlueprintReadOnly)
	FText QuestDisplayName;

	UPROPERTY(BlueprintReadOnly)
	FName QuestID;
};

class UQuestHeader_ListView;
class UQuestPanelWidget;

UCLASS()
class QUARTERVIEWGAME_API UQuestTabWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void InitQuestTab();

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnAddQuestHeader(const FName& QuestID, const FText& QuestDisplayName);
	
	UFUNCTION()
	void OnRemoveQuestHeader(const FName& QuestID);

	//현재 선택한 Quest정보가 변경될때 update
	UFUNCTION()
	void OnUpdateQuestPanel(const FName& QuestID);
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UQuestHeader_ListView> QuestHeaderListView;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UQuestPanelWidget> QuestPanel;
};
