// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuarterViewGameDataTable.h"
#include "UObject/NoExportTypes.h"
#include "ItemManager.generated.h"

class ADropStaticItem;

UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UItemManager : public UObject
{
	GENERATED_BODY()
	
public:
	void DropItemToCharacterID(const FName& CharacterID, const FVector& Location);
	
	const FConsumptionItemRow* GetConsumableItemDataTable(const FName& ItemID) const;
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ItemDropDataTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ADropStaticItem> StaticItemClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ConsumableItemDataTable;
};
