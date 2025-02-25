// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestStateWidget.h"

#include "QuarterViewGameInstance.h"
#include "WorldMapManager.h"
#include "Components/Image.h"

void UQuestStateWidget::ChangeState(EQuestState QuestState)
{
	UMaterialInstanceDynamic* IconMaterialInstance = IconImage->GetDynamicMaterial();

	UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetGameInstance());
	UWorldMapManager* WorldMapManager = QuarterViewGameInstance->GetWorldMapManager();
	
	switch (QuestState)
	{
		case EQuestState::Waiting :
			IconMaterialInstance->SetTextureParameterValue("Icon", ExclamationMarkIcon.Get());
			IconImage->SetColorAndOpacity(FColor::White);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			WorldMapManager->ChangeMapQuestIcon(OwnerUniqueID, ExclamationMarkIcon);
			break;
			
		case EQuestState::InProgress :
			IconMaterialInstance->SetTextureParameterValue("Icon", QuestionMarkIcon.Get());
			IconImage->SetColorAndOpacity(FColor(100.f, 100.f, 100.f));
			IconImage->SetVisibility(ESlateVisibility::Visible);
			WorldMapManager->ChangeMapQuestIcon(OwnerUniqueID, QuestionMarkIcon, FColor(100.f, 100.f, 100.f));
			break;
			
		case EQuestState::CompletionWaiting :
			IconMaterialInstance->SetTextureParameterValue("Icon", QuestionMarkIcon.Get());
			IconImage->SetColorAndOpacity(FColor::White);
			IconImage->SetVisibility(ESlateVisibility::Visible);
			WorldMapManager->ChangeMapQuestIcon(OwnerUniqueID, QuestionMarkIcon);
			break;
	
		case EQuestState::Lock :
			IconImage->SetVisibility(ESlateVisibility::Collapsed);
			WorldMapManager->ChangeMapQuestIcon(OwnerUniqueID, nullptr, FColor(0.f, 0.f, 0.f, 0.f));
		
		default:
		break;
	}
}

void UQuestStateWidget::SetOwnerID(int32 InOwnerUniqueID)
{
	OwnerUniqueID = InOwnerUniqueID;
}

const int UQuestStateWidget::GetOwnerID() const
{
	return OwnerUniqueID;
}

const UTexture* UQuestStateWidget::GetTexture() const
{
	UMaterialInstanceDynamic* IconMaterialInstance = IconImage->GetDynamicMaterial();
	
	FName QuestStateName = "QuestStateIcon";
	FHashedMaterialParameterInfo ParameterInfo;
	ParameterInfo.Name = NameToScriptName(QuestStateName);
	UTexture* Texture2D = nullptr;
	IconMaterialInstance->GetTextureParameterValue(ParameterInfo, Texture2D);

	return Texture2D;
}

FLinearColor UQuestStateWidget::GetColor() const
{
	return IconImage->GetColorAndOpacity();
}
