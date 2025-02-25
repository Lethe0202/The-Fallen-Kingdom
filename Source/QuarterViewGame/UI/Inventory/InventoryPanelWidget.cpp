// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPanelWidget.h"

#include "../Inventory/InventoryComponent.h"
#include "InventotySlotWidget.h"
#include "ItemMenuPanelWidget.h"
#include "../UI/Utility/TopBarWidget.h"

#include "Components/UniformGridPanel.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UInventoryPanelWidget::Init(UInventoryComponent* InventoryComponent)
{
	InventoryComponent->OnUpdateInventorySlotItem.AddDynamic(this, &UInventoryPanelWidget::OnUpdateItemSlot);
	InventoryComponent->OnUpdateSlotItemCoolingTime.AddDynamic(this, &UInventoryPanelWidget::OnUpdateItemCoolingTime);
	
	OnItemInventorySlotChanged.AddDynamic(InventoryComponent, &UInventoryComponent::HandleChangedSlot);
	OnUseItem.AddDynamic(InventoryComponent, &UInventoryComponent::HandleUseItem);
	OnItemSplit.AddDynamic(InventoryComponent, &UInventoryComponent::HandleSplitItem);
	OnDestroyItem.AddDynamic(InventoryComponent, &UInventoryComponent::HandleRemoveItem);

	const int32 InventorySize = InventoryComponent->GetInventorySize();

	for (int i = 0; i < InventorySize; ++i)
	{
		AddSlot();
	}
	
	TopBarWidget->SetTargetWidget(this);
}

void UInventoryPanelWidget::HandleChagnedUISlot(int32 Slot1, int32 Slot2)
{
	OnItemInventorySlotChanged.Broadcast(Slot1, Slot2);
}

void UInventoryPanelWidget::HandleOpenItemMenu(int32 ItemSlot, FText ItemName, EItemType ItemType)
{
	if (ItemOptionPanelClass != nullptr)
	{
		if (ItemOptionPanel == nullptr)
		{
			ItemOptionPanel = CreateWidget<UItemMenuPanelWidget>(this, ItemOptionPanelClass);
			ItemOptionPanel->AddToViewport(1);

			ItemOptionPanel->Init(this, ItemSlot, ItemName, ItemType);

			FVector2D CursorLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());

			ItemOptionPanel->SetPositionInViewport(CursorLocation);
		}
	}
}

void UInventoryPanelWidget::HandleUseItemUI(int32 ItemSlot)
{
	OnUseItem.Broadcast(ItemSlot);
}

void UInventoryPanelWidget::HandleSplitItemUI(int32 ItemSlot, int32 SplitQuantity)
{
	OnItemSplit.Broadcast(ItemSlot, SplitQuantity);
}

void UInventoryPanelWidget::HandleDestroyItemUI(int32 ItemSlot)
{
	OnDestroyItem.Broadcast(ItemSlot);
}

void UInventoryPanelWidget::OnUpdateItemSlot(int32 ItemSlot, const FInventoryItem& InventoryItem)
{
	if (InventoryItem.Quantity == 0)
	{
		ItemSlotList[ItemSlot]->ClearSlot();
	}
	else
	{
		ItemSlotList[ItemSlot]->InitItemSlotData(InventoryItem);
	}
}

void UInventoryPanelWidget::OnUpdateItemCoolingTime(int32 ItemSlot, float Percent, float CoolingTime)
{
	ItemSlotList[ItemSlot]->SetUICoolingTime(Percent, CoolingTime);
}

void UInventoryPanelWidget::AddSlot()
{
	UInventotySlotWidget* InventorySlotWidget = CreateWidget<UInventotySlotWidget>(this, InventorySlotWidgetClass);
	InventorySlotWidget->OnItemSlotChanged.AddDynamic(this, &UInventoryPanelWidget::HandleChagnedUISlot);
	InventorySlotWidget->OnRightClickSlot.AddDynamic(this, &UInventoryPanelWidget::HandleOpenItemMenu);
	InventorySlotWidget->OnUseItemSlot.AddDynamic(this, &UInventoryPanelWidget::HandleUseItemUI);
	
	InventoryUniformGridPanel->AddChildToUniformGrid(InventorySlotWidget, CurRowIndex, CurColumnIndex);

	++CurColumnIndex;

	if (CurColumnIndex >= ColumnIndex)
	{
		CurColumnIndex = 0;
		++CurRowIndex;
	}

	int32 SlotIndex = ItemSlotList.AddUnique(InventorySlotWidget);
	InventorySlotWidget->SetIndex(SlotIndex);
}

