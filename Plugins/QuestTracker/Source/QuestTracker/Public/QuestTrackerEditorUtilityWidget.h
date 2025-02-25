// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#if WITH_EDITOR
#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuestTrackerEditorUtilityWidget.generated.h"
#endif
/**
 * 
 */

#if WITH_EDITORONLY_DATA

USTRUCT()
struct FQuestTrackerInfo
{
	GENERATED_BODY()
	
	FString LevelPath;
	FString LevelName;
	FString ActorName;
	
	UPROPERTY()
	FVector QuestOwnerActorWorldLocation;
};

class UComboBoxString;
class UListView;
class UDataTable;
class UScrollBox;

UCLASS(Blueprintable, meta = (ShowWorldContextPin), config = Editor)
class QUESTTRACKER_API UQuestTrackerEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	virtual void NativePreConstruct() override; 
	virtual void NativeConstruct() override;
	
public:
	UFUNCTION(BlueprintCallable)
	void FindQuestGiverNPC();
	
	UFUNCTION(BlueprintCallable)
	void RefreshQuestGiverNPC();
	
protected:
	//받아온 아이템을 새로운 List로 패널에 뿌려줌
	UFUNCTION()
	void HandleAllItemShowNewPanel(const UQuestTrackerData* QuestTrackerData);
	
private:
	void InitQuestChapter();
	void ClearQuestTracker();

protected:
	UFUNCTION()
	void OnChapterChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
private:
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UComboBoxString> ChapterComboBoxString;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UListView> QuestListView;
	
	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UScrollBox> QuestTrackerAllItemScrollBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestTrackerAllItemWidget> QuestTrackerAllItemWidgetClass;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* QuestChapterDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	UDataTable* QuestDataTable;
	
	//ID, DisplayName
	UPROPERTY()
	TMap<FString, FName> QuestChapterMap;
	
	//ID, QuestGiverActor
	TMultiMap<FName, FQuestTrackerInfo> QuestOwnverMap;
};

#endif