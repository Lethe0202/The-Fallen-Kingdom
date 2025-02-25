// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProgressBarWidget.h"
#include "../Ability/AbilityComponent.h"
#include "../Ability/PlayerAbilityComponent.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAbilityProgressBarWidget::Init(UPlayerAbilityComponent* AbilityComponent)
{
	AbilityComponent->OnUpdateAbilityProgress.AddDynamic(this, &UAbilityProgressBarWidget::OnUpdateProgressBar);
}

void UAbilityProgressBarWidget::OnUpdateProgressBar(float Percent, float HoldTime, float MinPerfectZoneTime, float MaxPerfectZoneTime)
{
	auto HoldTimeString = FString::Printf(TEXT("%.1f"), HoldTime);
	
	if (Percent < 0)
	{
		this->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		this->SetVisibility(ESlateVisibility::Visible);
			
		AbilityProgressBar->SetPercent(Percent);
		HoldTimeTextBlock->SetText(FText::FromString(HoldTimeString));

		AbilityPerfectZoneImage->GetDynamicMaterial()->SetScalarParameterValue("Start", MinPerfectZoneTime);
		AbilityPerfectZoneImage->GetDynamicMaterial()->SetScalarParameterValue("End", MaxPerfectZoneTime);
	}
}
