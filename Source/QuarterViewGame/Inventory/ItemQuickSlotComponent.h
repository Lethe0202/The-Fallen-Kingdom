// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "QuarterViewGame/Interface/SaveableInterface.h"
#include "ItemQuickSlotComponent.generated.h"

class UInputMappingContext;
class UInputComponent;
class UEnhancedInputComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddedItemQuickSlot, const UInputAction*, InputAction, const FInventoryItem&, SlotItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemQuickSlot, const UInputAction*, InputAction, int32, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemQuickSlotToID, const UInputAction*, InputAction, const FName&, ItemID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUseItemQuickSlot,const FName&, ItemName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateItemQuickSlot, const FInventoryItem&, SlotItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemCooling, const FInventoryItem&, SlotItem, float, Percent, float, CoolintTime);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UItemQuickSlotComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this component's properties
	UItemQuickSlotComponent();
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void InitPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
	const FString GetSlotInputStringFromInputAction(const UInputAction* InputAction) const;

	/* ISaveableInterface */
	virtual void LoadData(USaveGameDataBase& LoadData) override;
	virtual void SaveData(/*out*/USaveGameDataBase*& SaveData) override;
	virtual ESaveDataType SaveDataType() override;
public:
	//인벤토리 슬롯을 퀵슬롯에 등록
	UFUNCTION()
	void HandleItemQuickSlotRegistered(UInputAction* SlotInputAction, int32 InventorySlotIndex);
	
	UFUNCTION()
	void HandleAddItemQuickSlot(const UInputAction* SlotInputAction, const FInventoryItem& InventoryItem);
	
	UFUNCTION()
	void HandleUpdateItemQuickSlot(const FInventoryItem& InventoryItem);
	
	UFUNCTION()
	void HandleUpdateItemCoolingTime(const FInventoryItem& InventoryItem, float Percent, float CoolingTime);
	
	UFUNCTION()
	void HandleChangedItemQuickSlot(const UInputAction* OriginInputAction, const UInputAction* TargetInputAction);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void Input_ItemQuickSlot(const FInputActionInstance& Value, const int32 Index);
	
public:
	FOnAddItemQuickSlot OnAddItemQuickSlot;
	FOnAddItemQuickSlotToID OnAddItemQuickSlotToID;
	FOnAddedItemQuickSlot OnAddedItemQuickSlot;
	FOnUseItemQuickSlot OnUseItemQuickSlot;
	
	//UI업데이트
	FOnUpdateItemQuickSlot OnUpdateItemQuickSlot;
	FOnUpdateItemCooling OnUpdateItemCooling;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> ItemQuickSlotInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInputAction>> ItemQuickSlotInputActionList;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 ItemQuickSlotSize;
	
	UPROPERTY()
	TArray<FName> QuickSlotItemName;
	
	UEnhancedInputComponent* PlayerEnhancedInputComponent;
};

