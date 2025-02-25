// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Blueprint/UserWidget.h"
#include "ItemQuickSlotWidget.generated.h"

class UImage;
class UTextBlock;
class UItemQuickSlotComponent;
class UInputAction;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemQuickSlotChanged, const UInputAction*, Slot1, const UInputAction*, Slot2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemQuickSlotRegistered, UInputAction*, SlotInputAction, int32, SlotIndex);

UCLASS()
class QUARTERVIEWGAME_API UItemQuickSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitItemQuickSlot(UItemQuickSlotComponent* ItemQuickSlotComponent);
	
protected:
	UFUNCTION()
	void OnAddedItemQuickSlot(const UInputAction* InputAction, const FInventoryItem& SlotItem);

	UFUNCTION()
	void OnUpdateItemQuickSlot(const FInventoryItem& SlotItem);

	UFUNCTION()
	void OnUpdateItemCooling( const FInventoryItem& SlotItem, float Percent, float CoolintTime);

protected:
	void ClearSlot();
	
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemQuickSlotRegistered OnItemQuickSlotRegistered;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnItemQuickSlotChanged OnItemQuickSlotChanged;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> ItemCoolingGlow;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuickSlotButton_Text;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity_Text;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ItemQuickSlotInputAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FName CoolingParameterName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	bool bHiddenCoolTime = false;
	
	EItemType SlotType;
};


