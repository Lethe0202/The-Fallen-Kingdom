// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemManager.h"

#include "QuarterViewGameDataTable.h"
#include "Kismet/KismetMathLibrary.h"
#include "QuarterViewGame/Item/DropStaticItem.h"

void UItemManager::DropItemToCharacterID(const FName& CharacterID, const FVector& Location)
{
	FItemDropTableRow* ItemDropTableRow = ItemDropDataTable->FindRow<FItemDropTableRow>(CharacterID, TEXT(""));
	
	if (ItemDropTableRow != nullptr)
	{
		for (int i = 0; i < ItemDropTableRow->ItemDropArray.Num(); ++i)
		{
			float RandFloat = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
			
			if (RandFloat <= ItemDropTableRow->ItemDropArray[0].DropRate)
			{
				ADropStaticItem* ItemActor = GetWorld()->SpawnActor<ADropStaticItem>(StaticItemClass, Location, FRotator(0, 0, 0));
				ItemActor->InitDropItem(ItemDropTableRow->ItemDropArray[0].ItemID);
				ItemActor->GetComponentByClass<UPrimitiveComponent>()->SetCastShadow(false);
			}
		}
	}
}

const FConsumptionItemRow* UItemManager::GetConsumableItemDataTable(const FName& ItemID) const
{
	if (ConsumableItemDataTable)
	{
		FConsumptionItemRow* ItemData = ConsumableItemDataTable->FindRow<FConsumptionItemRow>(ItemID, TEXT(""));

		return ItemData ? ItemData : nullptr;
	}
	
	return nullptr;
}
