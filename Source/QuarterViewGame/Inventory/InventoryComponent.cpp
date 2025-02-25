// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "QuestManager.h"
#include "../Item/ItemEffectBase.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/ItemManager.h"
#include "../Manager/SaveManager.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InventoryItemList.SetNum(InventorySize);
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool bCooldown = false;
	
	for (auto& Elem : ItemCurrentCoolingTime)
	{
		if (Elem.Value > 0.0f)
		{
			Elem.Value -= DeltaTime;
			for (int i = 0; i < InventorySize; i++)
			{
				if (InventoryItemList[i].Quantity > 0 && InventoryItemList[i].InventoryItemType == Elem.Key)
				{
					const float BaseCoolingTime = ItemCoolingTime[Elem.Key];
					const float ElapsedTime = BaseCoolingTime - Elem.Value;
					const float Percent = FMath::Clamp(ElapsedTime / BaseCoolingTime, 0.f, 1.f);
					
					OnUpdateSlotItemCoolingTime.Broadcast(i, Percent,Elem.Value);		
					OnUpdateItemCoolingTime.Broadcast(InventoryItemList[i], Percent, Elem.Value);
				}
			}
			
			bCooldown = true;
		}
	}
	
	for (auto It = ItemCurrentCoolingTime.CreateIterator(); It; ++It)
	{
		if (It->Value <= 0.0f)
		{
			It.RemoveCurrent();
		}
	}
	
	// 모든 쿨타임이 끝나면 Tick 비활성화
	if (!bCooldown)
	{
		SetComponentTickEnabled(false);
	}
}

void UInventoryComponent::OpenInventory()
{
	
}

void UInventoryComponent::AddItem(const FString& ItemID)
{
	if (!InventoryItemMultiMap.Contains(*ItemID))
	{
		UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
		const FConsumptionItemRow* ConsumptionItemRow = GameInstance->GetItemManager()->GetConsumableItemDataTable(*ItemID);
		
		if (ConsumptionItemRow != nullptr)
		{
			FInventoryItem NewInventoryItem;
			NewInventoryItem.ItemID = ConsumptionItemRow->ItemID;
			NewInventoryItem.ItemDisplayName = ConsumptionItemRow->DisplayName;
			NewInventoryItem.InventoryItemType = ConsumptionItemRow->ItemType;
			NewInventoryItem.MaxQuantity = ConsumptionItemRow->ItemQuantity;
			NewInventoryItem.DisplayDescription = ConsumptionItemRow->DisplayDescription;
			NewInventoryItem.Icon = ConsumptionItemRow->IconImage;
			NewInventoryItem.Quantity = 0;
			NewInventoryItem.Quantity++;
			NewInventoryItem.Effect = ConsumptionItemRow->Effect;
			
			for (int i = 0; i < InventoryItemList.Num(); ++i)
			{
				if (InventoryItemList[i].Quantity == 0)
				{
					InventoryItemList[i] = NewInventoryItem;
					break;
				}
			}
			
			int32 ItemIndex = InventoryItemList.IndexOfByKey(NewInventoryItem);
			InventoryItemMultiMap.Emplace(*ItemID, ItemIndex);
			OnUpdateInventorySlotItem.Broadcast(ItemIndex, NewInventoryItem);
			OnUpdateInventoryItem.Broadcast(NewInventoryItem);
		}
	}
	else
	{
		int32 InventoryItemIndex = InventoryItemMultiMap.FindRef(*ItemID);
		InventoryItemList[InventoryItemIndex].Quantity++;
		
		OnUpdateInventorySlotItem.Broadcast(InventoryItemIndex, InventoryItemList[InventoryItemIndex]);

		//추가된 아이템과 같은 아이템을 수집하고 broadcast
		FInventoryItem InventoryItem = InventoryItemList[InventoryItemIndex];
		int32 SumItemQuantity = 0;
		TArray<int32> AddItemIndices;
		InventoryItemMultiMap.MultiFind(InventoryItem.ItemID, AddItemIndices);
	
		for (int i = 0; i < AddItemIndices.Num(); ++i)
		{
			SumItemQuantity += InventoryItemList[AddItemIndices[i]].Quantity;
		}
		
		InventoryItem.Quantity = SumItemQuantity;
		OnUpdateInventoryItem.Broadcast(InventoryItem);
	}

	if (UQuarterViewGameInstance* QuarterViewGameInstance= Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UQuestManager* QuestManager = QuarterViewGameInstance->GetQuestManager())
		{
			FName ItemName = FName(*ItemID);
			QuestManager->NotifyQuestQuantity(GetOwner(), ItemName, 1);
		}
	}
}

void UInventoryComponent::HandleChangedSlot(int32 Slot1, int32 Slot2)
{
	if (Slot1 == Slot2)
	{
		return;
	}
	
	if (InventoryItemList[Slot1].ItemID == InventoryItemList[Slot2].ItemID)
	{
		MergeSlot(Slot1, Slot2);
	}
	else
	{
		ChangeSlot(Slot1, Slot2);
	}
	
	InventoryItemMultiMap.ValueSort([](const int32& A, const int32& B)
	{
		return A > B;
	});
}

void UInventoryComponent::HandleUseItem(int32 Slot1)
{
	TArray<int32> UsedItemIndices;
	InventoryItemMultiMap.MultiFind(InventoryItemList[Slot1].ItemID, UsedItemIndices);

	if (UsedItemIndices.IsEmpty())
	{
		return;
	}

	FInventoryItem UsedItem = InventoryItemList[Slot1];
	
	if (InventoryItemList[Slot1].Effect != nullptr)
	{
		if (InventoryItemList[Slot1].Effect->CoolingTime > 0)
		{
			//현재 아이템 쿨타임 상태
			if (ItemCurrentCoolingTime.Contains(InventoryItemList[Slot1].InventoryItemType))
			{
				return;
			}
			else
			{
				ItemCurrentCoolingTime.Add(UsedItem.InventoryItemType, UsedItem.Effect->CoolingTime);
				ItemCoolingTime.Add(UsedItem.InventoryItemType, UsedItem.Effect->CoolingTime);
				SetComponentTickEnabled(true);
			}
		}
		
		InventoryItemList[Slot1].Effect->ApplyEffect(Cast<ACharacter>(GetOwner()));	
		--InventoryItemList[Slot1].Quantity;
		
		if (InventoryItemList[Slot1].Quantity == 0)
		{
			InventoryItemMultiMap.Remove(InventoryItemList[Slot1].ItemID, Slot1);
			InventoryItemList[Slot1] = FInventoryItem();
		}
		
		//Inventory UI Update
		OnUpdateInventorySlotItem.Broadcast(Slot1, InventoryItemList[Slot1]);
	}
	
	FInventoryItem InventoryItem = InventoryItemList[UsedItemIndices[0]];
	int32 SumItemQuantity = 0;
	
	for (int i = 0; i < UsedItemIndices.Num(); ++i)
	{
		SumItemQuantity += InventoryItemList[UsedItemIndices[i]].Quantity;
	}
	
	InventoryItem.Quantity = SumItemQuantity;
	OnUpdateInventoryItem.Broadcast(InventoryItem);
}

void UInventoryComponent::HandleUseItemToID(const FName& ItemID)
{
	//사용된 아이템과 같은 ID를 가지는 Slot 색인
	TArray<int32> ItemIndex;
	InventoryItemMultiMap.MultiFind(ItemID, ItemIndex);

	if (ItemIndex.IsEmpty())
	{
		return;
	}
	
	int32 InventoryItemIndex = ItemIndex[0];

	FInventoryItem UsedItem = InventoryItemList[InventoryItemIndex];
	
	if (UsedItem.Effect != nullptr)
	{
		if (UsedItem.Effect->CoolingTime > 0)
		{
			//현재 아이템 쿨타임 상태
			if (ItemCurrentCoolingTime.Contains(UsedItem.InventoryItemType))
			{
				return;
			}
			else
			{
				ItemCurrentCoolingTime.Add(UsedItem.InventoryItemType, UsedItem.Effect->CoolingTime);
				ItemCoolingTime.Add(UsedItem.InventoryItemType, UsedItem.Effect->CoolingTime);
				SetComponentTickEnabled(true);
			}
		}
		
		InventoryItemList[InventoryItemIndex].Effect->ApplyEffect(Cast<ACharacter>(GetOwner()));	
		--InventoryItemList[InventoryItemIndex].Quantity;
		
		if (InventoryItemList[InventoryItemIndex].Quantity == 0)
		{
			InventoryItemMultiMap.Remove(InventoryItemList[InventoryItemIndex].ItemID, InventoryItemIndex);
			InventoryItemList[InventoryItemIndex] = FInventoryItem();
		}
		
		OnUpdateInventorySlotItem.Broadcast(InventoryItemIndex, InventoryItemList[InventoryItemIndex]);
	}
	
	FInventoryItem InventoryItem = InventoryItemList[ItemIndex[0]];
	int32 SumItemQuantity = 0;
	
	for (int i = 0; i < ItemIndex.Num(); ++i)
	{
		SumItemQuantity += InventoryItemList[ItemIndex[i]].Quantity;
	}
	
	InventoryItem.Quantity = SumItemQuantity;
	OnUpdateInventoryItem.Broadcast(InventoryItem);
}

void UInventoryComponent::HandleSplitItem(int32 Slot, int32 SplitQuantity)
{
	int EmptyIndex = -1;

	for (int i = 0; i < InventoryItemList.Num(); ++i)
	{
		if (InventoryItemList[i].Quantity == 0)
		{
			EmptyIndex = i;
			break;
		}
	}
	
	if (EmptyIndex == -1)
	{
		return;
	}
	
	FInventoryItem SplitInventoryItem = InventoryItemList[Slot];
	SplitInventoryItem.Quantity = SplitQuantity;
	InventoryItemList[EmptyIndex] = SplitInventoryItem;
	InventoryItemList[Slot].Quantity -= SplitQuantity;

	InventoryItemMultiMap.Add(InventoryItemList[Slot].ItemID, EmptyIndex);

	InventoryItemMultiMap.ValueSort([](const int32& A, const int32& B)
	{
		return A > B;
	});
	
	//기존아이템
	OnUpdateInventorySlotItem.Broadcast(Slot, InventoryItemList[Slot]);
	
	//Split
	OnUpdateInventorySlotItem.Broadcast(EmptyIndex, InventoryItemList[EmptyIndex]);
}

void UInventoryComponent::HandleRemoveItem(int32 Slot)
{
	FInventoryItem RemovedItem = InventoryItemList[Slot];
	InventoryItemMultiMap.Remove(RemovedItem.ItemID, Slot);
	
	InventoryItemList[Slot] = FInventoryItem();
	OnUpdateInventorySlotItem.Broadcast(Slot, InventoryItemList[Slot]);
	
	TArray<int32> ItemIndex;
	InventoryItemMultiMap.MultiFind(RemovedItem.ItemID, ItemIndex);
	int32 SumItemQuantity = 0;
	
	for (int i = 0; i < ItemIndex.Num(); ++i)
	{
		SumItemQuantity += InventoryItemList[ItemIndex[i]].Quantity;
	}
	
	RemovedItem.Quantity = SumItemQuantity;
	OnUpdateInventoryItem.Broadcast(RemovedItem);
}

void UInventoryComponent::HandleInventoryActionToIndex(const UInputAction* SlotInputAction, int32 Slot)
{
	FInventoryItem InventoryItem = InventoryItemList[Slot];
	
	TArray<int32> ItemIndex;
	InventoryItemMultiMap.MultiFind(InventoryItem.ItemID, ItemIndex);

	int32 SumItemQuantity = 0;
	
	for (int i = 0; i < ItemIndex.Num(); ++i)
	{
		SumItemQuantity += InventoryItemList[ItemIndex[i]].Quantity;
	}
	
	InventoryItem.Quantity = SumItemQuantity;
	
	OnInventoryItemAction.Broadcast(SlotInputAction, InventoryItem);
}

void UInventoryComponent::HandleInventoryActionToID(const UInputAction* SlotInputAction, const FName& ItemID)
{
	TArray<int32> ItemIndex;
	InventoryItemMultiMap.MultiFind(ItemID, ItemIndex);

	FInventoryItem InventoryItem;
	
	if (ItemIndex.IsEmpty())
	{
		OnInventoryItemAction.Broadcast(SlotInputAction, InventoryItem);
		return;
	}

	InventoryItem = InventoryItemList[ItemIndex[0]];
	
	int32 SumItemQuantity = 0;
	
	for (int i = 0; i < ItemIndex.Num(); ++i)
	{
		SumItemQuantity += InventoryItemList[ItemIndex[i]].Quantity;
	}
	
	InventoryItem.Quantity = SumItemQuantity;
	
	OnInventoryItemAction.Broadcast(SlotInputAction, InventoryItem);
}

void UInventoryComponent::ChangeSlot(int32 Slot1, int32 Slot2)
{
	if (InventoryItemList[Slot1].ItemID != "None")
	{
		InventoryItemMultiMap.Remove(InventoryItemList[Slot1].ItemID, Slot1);
		InventoryItemMultiMap.Add(InventoryItemList[Slot1].ItemID, Slot2);
	}

	if (InventoryItemList[Slot2].ItemID != "None")
	{
		InventoryItemMultiMap.Remove(InventoryItemList[Slot2].ItemID, Slot2);
		InventoryItemMultiMap.Add(InventoryItemList[Slot2].ItemID, Slot1);
	}

	InventoryItemList.Swap(Slot1, Slot2);

	OnUpdateInventorySlotItem.Broadcast(Slot1, InventoryItemList[Slot1]);
	OnUpdateInventorySlotItem.Broadcast(Slot2, InventoryItemList[Slot2]);
}

void UInventoryComponent::MergeSlot(int32 Slot1, int32 Slot2)
{
	InventoryItemList[Slot2].Quantity += InventoryItemList[Slot1].Quantity;
	InventoryItemList[Slot1] = FInventoryItem();

	InventoryItemMultiMap.Remove(InventoryItemList[Slot2].ItemID, Slot1);

	OnUpdateInventorySlotItem.Broadcast(Slot1, InventoryItemList[Slot1]);
	OnUpdateInventorySlotItem.Broadcast(Slot2, InventoryItemList[Slot2]);
}

void UInventoryComponent::LoadData(USaveGameDataBase& LoadData)
{
	InventoryItemMultiMap.Empty();
	
	UInventorySaveData* InventorySaveData = Cast<UInventorySaveData>(&LoadData);
	if (!InventorySaveData->InventoryItemList.IsEmpty())
	{
		InventoryItemList = InventorySaveData->InventoryItemList;

		for (int i = 0; i < InventoryItemList.Num(); ++i)
		{
			if (InventoryItemList[i].Quantity > 0)
			{
				InventoryItemMultiMap.Emplace(InventoryItemList[i].ItemID, i);
				OnUpdateInventorySlotItem.Broadcast(i, InventoryItemList[i]);
				OnUpdateInventoryItem.Broadcast(InventoryItemList[i]);
			}
		}
	}
}

void UInventoryComponent::SaveData(USaveGameDataBase*& SaveData)
{
	SaveData = NewObject<UInventorySaveData>();
	Cast<UInventorySaveData>(SaveData)->InventoryItemList = InventoryItemList;
}

ESaveDataType UInventoryComponent::SaveDataType()
{
	return ESaveDataType::Inventory;
}
