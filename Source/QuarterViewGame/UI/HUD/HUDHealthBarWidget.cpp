// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDHealthBarWidget.h"

#include "Component/CharacterStatusComponent.h"
#include "Components/ProgressBar.h"

void UHUDHealthBarWidget::Init_Implementation(UCharacterStatusComponent* CharacterStatusComponent)
{
	NativeInit(CharacterStatusComponent);
}

void UHUDHealthBarWidget::NativeInit(UCharacterStatusComponent* CharacterStatusComponent)
{
	CharacterStatusComponent->OnHealthChanged.AddDynamic(this, &UHUDHealthBarWidget::OnHealthChange);
	InitWidget();
}

void UHUDHealthBarWidget::InitWidget()
{
	//Empty
}

void UHUDHealthBarWidget::OnHealthChange(float NewPercent)
{
	UpdateHealthBar(NewPercent);
}

void UHUDHealthBarWidget::UpdateHealthBar(float NewPercent)
{
	HealthProgressBar->SetPercent(NewPercent);
}

