// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestPanelWidget.generated.h"

class UVerticalBox;
class UTextBlock;
class UHorizontalBox;
class UQuestObjectiveTextBlock;

UENUM(BlueprintType)
enum class EQuestPanelType : uint8
{
	Accept,
	InProgress,
	Completion,
};

UCLASS()
class QUARTERVIEWGAME_API UQuestPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ClearPanel();
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void InitQuestPanel(const FName& QuestID, AActor* Interactor, EQuestPanelType QuestPanelType);
	
protected:
	void InitInProgress(const FName& QuestID, AActor* Interactor);
	void InitAccept(const FName& QuestID);
	void InitCompletion(const FName& QuestID);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName CurQuestID;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> Reward_Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UHorizontalBox> 	ButtonAction_Horizontal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestObjective_Text;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestName_Text;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDetail_Text;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UVerticalBox> ObjectiveText_Vertical;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UQuestObjectiveTextBlock> ObjectiveTextWidgetClass;   
};
