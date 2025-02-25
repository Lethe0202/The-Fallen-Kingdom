// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "../Character/AIBossCharacter.h"
#include "HUDBossHealthBarWidget.h"
#include "Quest/QuestTabWidget.h"
#include "../UI/Inventory/InventoryPanelWidget.h"
#include "../Inventory/ItemQuickSlotComponent.h"
#include "../Ability/PlayerAbilityComponent.h"

#include "Components/CanvasPanelSlot.h"

void UPlayerHUD::InitAbilitySlot_Implementation(UPlayerAbilityComponent* AbilityComponent)
{
}

void UPlayerHUD::InitItemQuickSlot_Implementation(UItemQuickSlotComponent* ItemQuickSlotComponent)
{
    
}

void UPlayerHUD::InitHealthBar_Implementation(UCharacterStatusComponent* CharacterStatusComponent)
{
}

void UPlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();

    InventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
    AbilityPanel->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerHUD::InitInventoryPanel(UInventoryComponent* InventoryComponent)
{
    InventoryPanel->Init(InventoryComponent);
}

void UPlayerHUD::ToggleInventory()
{
    if (InventoryPanel)
    {
        if (InventoryPanel->IsVisible())
        {
            InventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            InventoryPanel->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UPlayerHUD::ToggleAbilityPanel()
{
    if (AbilityPanel)
    {
        if (AbilityPanel->IsVisible())
        {
            AbilityPanel->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            AbilityPanel->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UPlayerHUD::ToggleQuestPanel()
{
    if (QuestTab)
    {
        if (QuestTab->IsVisible())
        {
            QuestTab->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            QuestTab->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if (QuestTabWidgetClass != nullptr)
        {
            QuestTab = CreateWidget<UQuestTabWidget>(this, QuestTabWidgetClass);
            
            QuestTab->AddToViewport(1);
            QuestTab->InitQuestTab();
        }
    }
}

void UPlayerHUD::ToggleWorldMap()
{
    if (WorldMapWidget)
    {
        if (WorldMapWidget->IsVisible())
        {
            WorldMapWidget->SetVisibility(ESlateVisibility::Collapsed);
        }
        else
        {
            WorldMapWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
    else
    {
        if (WorldMapWidgetClass != nullptr)
        {
            WorldMapWidget = CreateWidget<UUserWidget>(this, WorldMapWidgetClass);

            WorldMapWidget->AddToViewport(1);
        }
    }
}

void UPlayerHUD::InitTopBossHpBar(AAIBossCharacter* AIBossCharacter)
{
    TopBossHealthBar = CreateWidget<UHUDBossHealthBarWidget>(this, TopBossHealthBarClass);
    TopBossHealthBar->Init(AIBossCharacter->GetCharacterStatusComponent());
    TopBossHealthBar->SetHealthBarNum(10); //임시
    TopBossHealthBar->SetBossDisplayName(AIBossCharacter->GetCharacterDisplayName());
    TopBossHealthBar->AddToViewport(1);
    
    if (UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(TopBossHealthBar->Slot))
    {
        CanvasPanelSlot->SetAlignment(FVector2D(0.5, 0.5));
        CanvasPanelSlot->SetAnchors(FAnchors(0.5, 0.5));
    }
}

void UPlayerHUD::RemoveTopBossHpBar()
{
    TopBossHealthBar->RemoveFromParent();
    TopBossHealthBar = nullptr;
}

const bool UPlayerHUD::IsVisibleBossHPBar(const FName& BossID) const
{
    if (TopBossHealthBar == nullptr)
    {
        return false;
    }
    
    return true;
}
