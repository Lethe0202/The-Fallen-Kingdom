// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemQuickSlotWidget.h"
#include "../Inventory/ItemQuickSlotComponent.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "QuarterViewGame/Inventory/InventoryComponent.h"

void UItemQuickSlotWidget::InitItemQuickSlot(UItemQuickSlotComponent* ItemQuickSlotComponent)
{
	ItemQuickSlotComponent->OnAddedItemQuickSlot.AddDynamic(this, &UItemQuickSlotWidget::OnAddedItemQuickSlot);
	ItemQuickSlotComponent->OnUpdateItemQuickSlot.AddDynamic(this, &UItemQuickSlotWidget::OnUpdateItemQuickSlot);
	ItemQuickSlotComponent->OnUpdateItemCooling.AddDynamic(this, &UItemQuickSlotWidget::OnUpdateItemCooling);
	
	OnItemQuickSlotRegistered.AddDynamic(ItemQuickSlotComponent, &UItemQuickSlotComponent::HandleItemQuickSlotRegistered);
	
	OnItemQuickSlotChanged.AddDynamic(ItemQuickSlotComponent, &UItemQuickSlotComponent::HandleChangedItemQuickSlot);
	
	const FString KeyString = ItemQuickSlotComponent->GetSlotInputStringFromInputAction(ItemQuickSlotInputAction);
	ItemQuickSlotButton_Text->SetText(FText::FromString(KeyString));
	ItemQuantity_Text->SetText(FText());
}

void UItemQuickSlotWidget::OnAddedItemQuickSlot(const UInputAction* InputAction, const FInventoryItem& SlotItem)
{
	if (InputAction != ItemQuickSlotInputAction)
	{
		return;
	}
	
	if (SlotItem.Quantity == 0)
	{
		ClearSlot();
	}
	else
	{
		ItemIcon->SetBrushFromTexture(SlotItem.Icon.Get());
		ItemQuantity_Text->SetText(FText::FromString(FString::FromInt(SlotItem.Quantity)));
		SlotType = SlotItem.InventoryItemType;
	}
}

void UItemQuickSlotWidget::OnUpdateItemQuickSlot(const FInventoryItem& SlotItem)
{
	if (ItemQuantity_Text->GetText().IsEmpty())
	{
		return;
	}
	
	if (SlotItem.Quantity == 0)
	{
		ClearSlot();
	}
	else
	{
		ItemQuantity_Text->SetText(FText::FromString(FString::FromInt(SlotItem.Quantity)));	
	}
}

void UItemQuickSlotWidget::OnUpdateItemCooling(const FInventoryItem& SlotItem, float Percent, float CoolintTime)
{
	if (SlotType == SlotItem.InventoryItemType)
	{
		ItemCoolingGlow->SetVisibility(ESlateVisibility::Visible);
		ItemCoolingGlow->GetDynamicMaterial()->SetScalarParameterValue(CoolingParameterName, Percent);	
	}
	else
	{
		ItemCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UItemQuickSlotWidget::ClearSlot()
{
	ItemIcon->SetBrushFromTexture(nullptr);
	ItemQuantity_Text->SetText(FText());
	ItemCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);
	SlotType = EItemType::None;
}

