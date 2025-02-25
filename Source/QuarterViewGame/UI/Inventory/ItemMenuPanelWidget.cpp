// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemMenuPanelWidget.h"
#include "InventoryPanelWidget.h"
#include "../Inventory/InventoryComponent.h"
#include "DataAsset/ItemOption_DataAsset.h"

void UItemMenuPanelWidget::Init_Implementation(UInventoryPanelWidget* InventoryPanelWidget, int32 Index, const FText& InItemName, EItemType InItemType)
{
	ParentPanelWidget = InventoryPanelWidget;
	ItemIndex = Index;

	//메뉴 추가는 블루프린트에서 수행
}
