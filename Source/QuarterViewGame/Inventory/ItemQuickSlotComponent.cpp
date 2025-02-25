// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemQuickSlotComponent.h"
#include "../Manager/SaveManager.h"

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InventoryComponent.h"

// Sets default values for this component's properties
UItemQuickSlotComponent::UItemQuickSlotComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UItemQuickSlotComponent::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(ItemQuickSlotInputMappingContext, 1);
	}
	
	QuickSlotItemName.Init("", ItemQuickSlotSize);

	InitPlayerInputComponent(PlayerController->InputComponent);
}

// Called every frame
void UItemQuickSlotComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemQuickSlotComponent::InitPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (PlayerEnhancedInputComponent != nullptr)
	{
		for (int32 i = 0; i < ItemQuickSlotInputActionList.Num(); ++i)
		{
			if (QuickSlotItemName.IsValidIndex(i))
			{
				PlayerEnhancedInputComponent->BindAction(ItemQuickSlotInputActionList[i], ETriggerEvent::Started, this, &UItemQuickSlotComponent::Input_ItemQuickSlot, i);
			}
		}
	}
}

void UItemQuickSlotComponent::Input_ItemQuickSlot(const FInputActionInstance& Value, const int32 Index)
{
	//퀵슬롯 아이템 사용
	OnUseItemQuickSlot.Broadcast(QuickSlotItemName[Index]);
}

void UItemQuickSlotComponent::HandleItemQuickSlotRegistered(UInputAction* SlotInputAction, int32 InventorySlotIndex)
{
	//퀵슬롯 업데이트
	OnAddItemQuickSlot.Broadcast(SlotInputAction, InventorySlotIndex);
}

//스왑할때 2번 불려서 총 OnAddedItemQuickSlot가 4번 브로드캐스트 됨 
void UItemQuickSlotComponent::HandleAddItemQuickSlot(const UInputAction* SlotInputAction, const FInventoryItem& InventoryItem)
{
	//아이템이 남아있을떄
	if (InventoryItem.ItemID != "None")
	{
		int32 QuickSlotIndex = ItemQuickSlotInputActionList.IndexOfByKey(SlotInputAction);
		QuickSlotItemName[QuickSlotIndex] = InventoryItem.ItemID;
		
		for (int i = 0; i < ItemQuickSlotSize; i++)
		{
			if (QuickSlotItemName[i] == InventoryItem.ItemID)
			{
				OnAddedItemQuickSlot.Broadcast(ItemQuickSlotInputActionList[i], InventoryItem);			
			}
		}
	}
	//아이템을 모두 소비했을때 또는 InventoryItem가 존재하지않을때
	else
	{
		int32 QuickSlotIndex = ItemQuickSlotInputActionList.IndexOfByKey(SlotInputAction);
		FName ItemID = QuickSlotItemName[QuickSlotIndex];
		QuickSlotItemName[QuickSlotIndex] = "";
		
		for (int i = 0; i < ItemQuickSlotSize; i++)
		{
			if (QuickSlotItemName[i] == ItemID)
			{
				OnAddedItemQuickSlot.Broadcast(ItemQuickSlotInputActionList[i], InventoryItem);			
			}
		}
	}
}

void UItemQuickSlotComponent::HandleUpdateItemQuickSlot(const FInventoryItem& InventoryItem)
{
	for (int i = 0; i < ItemQuickSlotSize; i++)
	{
		if (InventoryItem.ItemID == QuickSlotItemName[i])
		{
			//사용 후 UI 등 업데이트
			OnUpdateItemQuickSlot.Broadcast(InventoryItem);
		}
	}
}

void UItemQuickSlotComponent::HandleUpdateItemCoolingTime(const FInventoryItem& InventoryItem, float Percent, float CoolingTime)
{
	for (int i = 0; i < ItemQuickSlotSize; i++)
	{
		if (InventoryItem.ItemID == QuickSlotItemName[i])
		{
			//사용 후 UI 등 업데이트
			OnUpdateItemCooling.Broadcast(InventoryItem, Percent, CoolingTime);
		}
	}
}

void UItemQuickSlotComponent::HandleChangedItemQuickSlot(const UInputAction* OriginInputAction, const UInputAction* TargetInputAction)
{
	//슬롯이 아닌 다른곳으로 변경(마우스 드래그앤 드랍) 했을경우
	if (TargetInputAction == nullptr)
	{
		int32 QuickSlotIndex = ItemQuickSlotInputActionList.IndexOfByKey(OriginInputAction);
		QuickSlotItemName[QuickSlotIndex] = "";

		//UI업데이트
		OnAddedItemQuickSlot.Broadcast(OriginInputAction, FInventoryItem());
	}
	//2개의 슬롯 변경
	else
	{
		int32 OriginQuickSlotIndex = ItemQuickSlotInputActionList.IndexOfByKey(OriginInputAction);
		int32 TargetQuickSlotIndex = ItemQuickSlotInputActionList.IndexOfByKey(TargetInputAction);
		QuickSlotItemName.Swap(OriginQuickSlotIndex, TargetQuickSlotIndex);
		
		//스왑 델리게이트 브로드캐스트
		OnAddItemQuickSlotToID.Broadcast(OriginInputAction, QuickSlotItemName[OriginQuickSlotIndex]);
		OnAddItemQuickSlotToID.Broadcast(TargetInputAction, QuickSlotItemName[TargetQuickSlotIndex]);
	}
}

const FString UItemQuickSlotComponent::GetSlotInputStringFromInputAction(const UInputAction* InputAction) const
{
	FString KeyString = "";

	const TArray<FEnhancedActionKeyMapping>& ItemQuickSlotInputMappings = ItemQuickSlotInputMappingContext->GetMappings();
	
	for(const FEnhancedActionKeyMapping& SlotInputMapping : ItemQuickSlotInputMappings)
	{
		if (SlotInputMapping.Action == InputAction)
		{
			KeyString = SlotInputMapping.Key.GetDisplayName().ToString();
			break;
		}
	}
	
	return KeyString;
}

void UItemQuickSlotComponent::LoadData(USaveGameDataBase& LoadData)
{
	UQuickSlotSaveData* QuickSlotSaveData = Cast<UQuickSlotSaveData>(&LoadData);
	QuickSlotItemName = QuickSlotSaveData->QuickSlotItemName;
	
	for (int i = 0; i < ItemQuickSlotSize; i++)
	{
		OnAddItemQuickSlotToID.Broadcast(ItemQuickSlotInputActionList[i], QuickSlotItemName[i]);
	}
}

void UItemQuickSlotComponent::SaveData(USaveGameDataBase*& SaveData)
{
	SaveData = NewObject<UQuickSlotSaveData>();
	Cast<UQuickSlotSaveData>(SaveData)->QuickSlotItemName = QuickSlotItemName;
}

ESaveDataType UItemQuickSlotComponent::SaveDataType()
{
	return ESaveDataType::QuickSlot;
}
