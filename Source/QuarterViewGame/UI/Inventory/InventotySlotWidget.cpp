// Fill out your copyright notice in the Description page of Project Settings.


#include "InventotySlotWidget.h"

#include "../../Inventory/InventoryComponent.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInventotySlotWidget::NativeConstruct()
{
	IconImage->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventotySlotWidget::InitItemSlotData(const FInventoryItem& Item)
{
	UTexture2D* Texture = Item.Icon.LoadSynchronous();

	IconImage->SetBrushFromTexture(Texture);
	IconImage->SetVisibility(ESlateVisibility::Visible);
	Quantity_TextBlock->SetText(FText::FromString(FString::FromInt(Item.Quantity)));
	
	Quantity = Item.Quantity;
	ItemName = Item.ItemDisplayName;
	ItemType = Item.InventoryItemType;
}

void UInventotySlotWidget::ClearSlot()
{
	Quantity_TextBlock->SetText(FText::FromString(""));
	IconImage->SetVisibility(ESlateVisibility::Collapsed);
	ItemCoolingGlow->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventotySlotWidget::SetIndex(int32 Index)
{
	SlotIndex = Index;
}

void UInventotySlotWidget::SetUICoolingTime(float Percent, float CoolingTime)
{
	ItemCoolingGlow->SetVisibility(ESlateVisibility::Visible);
	ItemCoolingGlow->GetDynamicMaterial()->SetScalarParameterValue(CoolingParameterName, Percent);
}
