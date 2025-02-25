// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Inventory/InventoryComponent.h"
#include "InventotySlotWidget.generated.h"

class UImage;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemUISlotChanged, int32, Slot1, int32, Slot2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseItemSlot, int32, ItemSlot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRightClickSlot, int32, ItemSlot, FText, ItemName, EItemType, ItemType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSplitSlot, int32, Slot, int32, Quantity);

UCLASS()
class QUARTERVIEWGAME_API UInventotySlotWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	void InitItemSlotData(const FInventoryItem& Item);
	void ClearSlot();
	
public:
	void SetIndex(int32 Index);
	void SetUICoolingTime(float Percent, float CoolingTime);

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemUISlotChanged OnItemSlotChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnUseItemSlot OnUseItemSlot;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnRightClickSlot OnRightClickSlot;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemSplitSlot OnItemSplit;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> ItemCoolingGlow;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Quantity_TextBlock;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	int32 SlotIndex;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Quantity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText ItemName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemType ItemType;

private:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FName CoolingParameterName;
};
