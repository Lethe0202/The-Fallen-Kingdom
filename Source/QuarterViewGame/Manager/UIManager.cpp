// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"

#include "Blueprint/UserWidget.h"
#include "HUD/BaseHUDQuestState.h"

void UUIManager::QuestAccept(const FText& QuestDisplayName)
{
	if (QuestAcceptWidgetClass)
	{
		UBaseHUDQuestState* HUDQuestState = CreateWidget<UBaseHUDQuestState>(GetWorld(), QuestAcceptWidgetClass);
		HUDQuestState->SetDescription(QuestDisplayName);

		HUDQuestState->AddToViewport();
	}
}

void UUIManager::QuestObjectiveConditionUpdate(const FText& TaskConditionText)
{
	if (QuestConditionWidgetClass)
	{
		if (QuestConditionClass)
		{
			QuestConditionClass->RemoveFromParent();	
		}
		
		QuestConditionClass = CreateWidget<UBaseHUDQuestState>(GetWorld(), QuestConditionWidgetClass);
		QuestConditionClass->SetDescription(TaskConditionText);
		QuestConditionClass->AddToViewport();
	}
}

void UUIManager::QuestObjectiveComplete(const FText& TaskConditionDisplayText)
{
	if (QuestConditionWidgetClass)
	{
		if (QuestConditionClass)
		{
			QuestConditionClass->RemoveFromParent();	
		}
		
		QuestConditionClass = CreateWidget<UBaseHUDQuestState>(GetWorld(), QuestConditionWidgetClass);

		FString string  = TaskConditionDisplayText.ToString();
		string.Append(" Complete");
		QuestConditionClass->SetDescription(FText::FromString(string));
		QuestConditionClass->AddToViewport();
	}
}

void UUIManager::QuestSuccess(const FText& QuestDisplayName)
{
	if (QuestSuccessWidgetClass)
	{
		UBaseHUDQuestState* HUDQuestState = CreateWidget<UBaseHUDQuestState>(GetWorld(), QuestSuccessWidgetClass);
		HUDQuestState->SetDescription(QuestDisplayName);

		HUDQuestState->AddToViewport();
	}
}

UTexture2D* UUIManager::GetAbilityActivationTypeIcon(EAbilityActivationType AbilityActivationType) const
{
	if (AbilityActivationTypeIcon.Contains(AbilityActivationType))
	{
		return AbilityActivationTypeIcon[AbilityActivationType].LoadSynchronous();
	}
	
	return nullptr;
}

void UUIManager::RegisterPlayerHUD(UPlayerHUD* HUD)
{
	PlayerHUD = HUD;
}

void UUIManager::ViewDeadScreen()
{
	if (UUserWidget* DeadScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DeadScreenWidgetClass))
	{
		DeadScreenWidget->AddToViewport(0);
	}
}

const TWeakObjectPtr<UPlayerHUD> UUIManager::GetPlayerHUD() const
{
	return PlayerHUD;
}
