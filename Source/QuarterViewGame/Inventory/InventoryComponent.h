// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Interface/SaveableInterface.h"
#include "InventoryComponent.generated.h"

class UItemEffectBase;
enum class ESaveDataType : uint8;

UENUM(BlueprintType)
enum class EItemType : uint8
{
    None,
    Postion,
    Equipped,
    Food,
    Misc
};

UENUM(BlueprintType)
enum class ERarity : uint8
{
    Common,
    Rare,
    Epic
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()
    
public:
    FInventoryItem()
    {
    }
    
    bool operator==(const FInventoryItem& Ohter) const
    {
        return this->ItemID == Ohter.ItemID;
    }
    
public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FName ItemID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FText ItemDisplayName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    FText DisplayDescription;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    EItemType InventoryItemType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 Quantity = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    int32 MaxQuantity = 0;
    
    //데이터가 많아질 경우 매니저클래스로 캐싱
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
    TObjectPtr<UItemEffectBase> Effect;
};

USTRUCT(BlueprintType)
struct FItemCoolingTimer
{
    GENERATED_BODY()
    
public:
    void OnTimer();
    
    FTimerHandle ItemTimerHandle;
    float CoolingTime = 0;
    float CurrentCoolingTime = 0;
};

USTRUCT(BlueprintType)
struct FInventoryConsumableItem : public FInventoryItem
{
    GENERATED_BODY()

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateInventorySlotItem, int32, ItemSlot, const FInventoryItem&, InventoryItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateInventoryItem, const FInventoryItem&, InventoryItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryItemAction, const UInputAction*, SlotInputAction, const FInventoryItem&, InventoryItem);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateSlotItemCoolingTime, int32, ItemSlot, float, Percent, float, CoolingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnUpdateItemCoolingTime, const FInventoryItem&, InventoryItem, float, Percent, float, CoolingTime);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UInventoryComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()
    
public:
	// Sets default values for this component's properties
	UInventoryComponent();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenInventory();

    UFUNCTION(BlueprintCallable)
    void AddItem(const FString& ItemID);
    
    /* ISaveableInterface */
    virtual void LoadData(USaveGameDataBase& LoadData) override;
    virtual void SaveData(/*out*/USaveGameDataBase*& SaveData) override;
    virtual ESaveDataType SaveDataType() override;
    
protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    
public:
    UFUNCTION()
    void HandleChangedSlot(int32 Slot1, int32 Slot2);
    
    UFUNCTION()
    void HandleUseItem(int32 Slot1);
    
    UFUNCTION()
    void HandleUseItemToID(const FName& ItemID);
    
    UFUNCTION()
    void HandleSplitItem(int32 Slot, int32 SplitQuantity);
    
    UFUNCTION()
    void HandleRemoveItem(int32 Slot);
    
    UFUNCTION()
    void HandleInventoryActionToIndex(const UInputAction* SlotInputAction, int32 Slot);
    
    UFUNCTION()
    void HandleInventoryActionToID(const UInputAction* SlotInputAction, const FName& ItemID);
    
private:
    void ChangeSlot(int32 Slot1, int32 Slot2);
    void MergeSlot(int32 Slot1, int32 Slot2);
    
public:
    const int32 GetInventorySize() const { return InventorySize; }
    
public:
    /**
     * 인베토리 슬롯과 인벤토리 아이템 두가지로 델리게이트를 나눠둔 이유는
     * 아이템슬롯에 대한 이벤트와
     * 아이템 자체에 대한 이벤트로 나눠서 액션을 취하기 위함
     */
    //인벤토리 슬롯 Delegate
    FOnUpdateInventorySlotItem OnUpdateInventorySlotItem;
    
    //인벤토리 아이템 Delegate
    FOnUpdateInventoryItem OnUpdateInventoryItem;

    //인벤토리 액션 Delegate
    FOnInventoryItemAction OnInventoryItemAction;

    //인벤토리 슬롯 쿨타임 Delegate
    FOnUpdateSlotItemCoolingTime OnUpdateSlotItemCoolingTime;

    //인벤토리 아이템 쿨타임 Delegate
    FOnUpdateItemCoolingTime OnUpdateItemCoolingTime;
    
private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
    int32 InventorySize;

    //<ItemID, InventorySlotIndex> --> 아이템 슬롯Index 캐싱
    TMultiMap<FName, int32> InventoryItemMultiMap;
    
    //아이템 타입별 쿨타임 Map
    TMap<EItemType, float> ItemCurrentCoolingTime;
    TMap<EItemType, float> ItemCoolingTime;
    
    UPROPERTY()
    TArray<FInventoryItem> InventoryItemList;
};
