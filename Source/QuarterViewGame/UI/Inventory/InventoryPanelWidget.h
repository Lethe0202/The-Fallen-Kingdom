// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryPanelWidget.generated.h"

enum class EItemType : uint8;
class UCanvasPanel;

class UInventotySlotWidget;
class UInventoryComponent;
class UTopBarWidget;
class UUniformGridPanel;
class UItemMenuPanelWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemInventorySlotChanged, int32, Slot1, int32, Slot2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseItem, int32, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSplit, int32, Slot, int32, Quantity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyItem, int32, ItemSlot);

UCLASS()
class QUARTERVIEWGAME_API UInventoryPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Init(UInventoryComponent* InventoryComponent);

	UFUNCTION(BlueprintCallable)
	void HandleChagnedUISlot(int32 Slot1, int32 Slot2);

	UFUNCTION(BlueprintCallable)
	void HandleOpenItemMenu(int32 ItemSlot, FText ItemName, EItemType ItemType);

	UFUNCTION(BlueprintCallable)
	void HandleUseItemUI(int32 ItemSlot);

	UFUNCTION(BlueprintCallable)
	void HandleSplitItemUI(int32 ItemSlot, int32 SplitQuantity);

	UFUNCTION(BlueprintCallable)
	void HandleDestroyItemUI(int32 ItemSlot);

public:
	UFUNCTION(BlueprintCallable)
	UCanvasPanel* GetCanvasPanel() { return CanvasPanel; }
	
protected:
	UFUNCTION()
	void OnUpdateItemSlot(int32 ItemSlot, const FInventoryItem& InventoryItem);

	UFUNCTION()
	void OnUpdateItemCoolingTime(int32 ItemSlot, float Percent , float CoolingTime);
	
	FOnItemInventorySlotChanged OnItemInventorySlotChanged;
	FOnUseItem OnUseItem;
	FOnItemSplit OnItemSplit;
	FOnDestroyItem OnDestroyItem;

private:
	void AddSlot();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 ColumnIndex;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTopBarWidget> TopBarWidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryUniformGridPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UInventotySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInventotySlotWidget>> ItemSlotList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemMenuPanelWidget> ItemOptionPanelClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UItemMenuPanelWidget> ItemOptionPanel;

private:
	int32 CurRowIndex = 0;
	int32 CurColumnIndex = 0;
};
