// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySlotWidget.h"
#include "../Character/BaseCharacter.h"
#include "../Ability/BaseAbility.h"
#include "../Ability/PlayerAbilityComponent.h"
#include "../Ability/AbilityComponent.h"

#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/UIManager.h"

#include "InputAction.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UAbilitySlotWidget::Init(UPlayerAbilityComponent* AbilityComponent)
{
	OnAbilitySlotChanged.AddDynamic(AbilityComponent, &UPlayerAbilityComponent::HandleAbilitySlotChange);
	OnAbilitySlotRegistered.AddDynamic(AbilityComponent, &UPlayerAbilityComponent::HandleRegisterAbilitySlot);
	
	AbilityComponent->OnAbilitySlotUpdate.AddDynamic(this, &UAbilitySlotWidget::OnUpdateAbilitySlot);
	
	OnUpdateAbilitySlot(AbilityInputAction, AbilityComponent);
}

void UAbilitySlotWidget::OnUpdateAbilitySlot(const UInputAction* InputAction, UPlayerAbilityComponent* AbilityComponent)
{
	if (AbilityInputAction != InputAction)
	{
		return;
	}
	
	UBaseAbility* Ability = AbilityComponent->GetAbilityFromInputAction(AbilityInputAction);
	if (Ability != nullptr)
	{
		const FAbilityStaticData& AbilityStaticData = Ability->GetAbilityStaticData();

		UTexture2D* Texture = AbilityStaticData.IconTexture.LoadSynchronous();
		if (Texture)
		{
			IconImage->SetBrushFromTexture(Texture);
			IconImage->SetVisibility(ESlateVisibility::Visible);

			InitAbilityInputIcon(Ability->GetAbilityStaticData().AbilityActivationType);
		}
		else
		{
			IconImage->SetBrushFromTexture(nullptr);
			IconImage->SetVisibility(ESlateVisibility::Collapsed); 
		}
		
		Ability->OnUpdateAbilityCooling.AddDynamic(this, &UAbilitySlotWidget::OnUpdateAbilityCoolingTimer);

		if (Ability->GetAbilityStaticData().AbilityActivationType == EAbilityActivationType::Chain)
		{
			Ability->OnUpdateAbilityChainCooling.AddDynamic(this, &UAbilitySlotWidget::OnUpdateAbilityChainCoolingTimer);
		}
		
		FString KeyString = AbilityComponent->GetAbilityInputKeyFromInputAction(AbilityInputAction);
		AbilityButtonText->SetText(FText::FromString(KeyString));

		if (bHiddenCoolTime == true)
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		IconImage->SetBrushFromTexture(nullptr);
		IconImage->SetVisibility(ESlateVisibility::Collapsed);
		AbilityInputIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	CoolingCountText->SetVisibility(ESlateVisibility::Collapsed);
	AbilityCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);
	AbilityChainCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);
}

void UAbilitySlotWidget::InitAbilityInputIcon(EAbilityActivationType AbilityActivationType)
{
	UUIManager* UIManager = Cast<UQuarterViewGameInstance>(GetGameInstance())->GetUIManager();
	UTexture2D* InputIcon = UIManager->GetAbilityActivationTypeIcon(AbilityActivationType);

	if (InputIcon != nullptr)
	{
		AbilityInputIcon->SetBrushFromTexture(InputIcon);
		AbilityInputIcon->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		AbilityInputIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAbilitySlotWidget::OnUpdateAbilityCoolingTimer(float Percent, float RemainCoolingTime)
{
	FString RemainCoolingTimeString = FString::FromInt(RemainCoolingTime) + "s";
	FText RemainCoolingTimeText = FText::FromString(RemainCoolingTimeString);

	CoolingCountText->SetText(RemainCoolingTimeText);
	AbilityCoolingGlow->GetDynamicMaterial()->SetScalarParameterValue(CoolingParameterName, Percent);
	
	if (RemainCoolingTime <= 0)
	{
		CoolingCountText->SetVisibility(ESlateVisibility::Collapsed);
		AbilityCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);

		if (bHiddenCoolTime == true)
		{
			this->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		if (bHiddenCoolTime == true)
		{
			this->SetVisibility(ESlateVisibility::Visible);
		}

		AbilityCoolingGlow->SetVisibility(ESlateVisibility::Visible);
		CoolingCountText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UAbilitySlotWidget::OnUpdateAbilityChainCoolingTimer(float Percent, float RemainCoolingTime)
{
	FString RemainCoolingTimeString = FString::FromInt(RemainCoolingTime);
	FText RemainCoolingTimeText = FText::FromString(RemainCoolingTimeString);

	AbilityChainCoolingGlow->GetDynamicMaterial()->SetScalarParameterValue(CoolingParameterName, Percent);

	if (RemainCoolingTime <= 0)
	{
		AbilityChainCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		AbilityChainCoolingGlow->SetVisibility(ESlateVisibility::Visible);
	}
}
