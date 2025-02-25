// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveManager.h"
#include "GameFramework/Character.h"
#include "../Interface/SaveableInterface.h"
#include "Kismet/GameplayStatics.h"

void USaveManager::InitSaveManager(APlayerController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}
	
	if (ACharacter* Character = PlayerController->GetCharacter())
	{
		TSet<UActorComponent*> Components = Character->GetComponents();
		for (UActorComponent* Component : Components)
		{
			if (ISaveableInterface* Saveable = Cast<ISaveableInterface>(Component))
			{
				RegisteredComponents.Add(Saveable->SaveDataType(), Component);
			}
		}
	}
}

void USaveManager::LoadGameData()
{
	if (USaveGameData* LoadData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0)))
	{
		for (auto& Component : RegisteredComponents)
		{
			if (!Component.Value->IsValidLowLevel())
			{
				continue;
			}
			
			if (ISaveableInterface* Saveable = Cast<ISaveableInterface>(Component.Value))
			{
				ESaveDataType SaveDataType = Saveable->SaveDataType();
				
				if (SaveDataType == ESaveDataType::Inventory)
				{
					UInventorySaveData* InventorySaveData = NewObject<UInventorySaveData>();
					InventorySaveData->InventoryItemList = LoadData->InventorySaveData;	
					Saveable->LoadData(*InventorySaveData);
				}
				else if (SaveDataType == ESaveDataType::AbilitySlot)
				{
					UAbilitySlotSaveData* AbilitySaveData = NewObject<UAbilitySlotSaveData>();
					AbilitySaveData->AbilitySlotClassList = LoadData->AbilityClassSaveData;	
					Saveable->LoadData(*AbilitySaveData);
				}
				else if (SaveDataType == ESaveDataType::QuickSlot)
				{
					UQuickSlotSaveData* QuickSlotSaveData = NewObject<UQuickSlotSaveData>();
					QuickSlotSaveData->QuickSlotItemName = LoadData->QuickSlotNameSaveData;
					Saveable->LoadData(*QuickSlotSaveData);
				}
				else if (SaveDataType == ESaveDataType::Quest)
				{
					UQuestInfoSaveData* QuestInfoSaveData = NewObject<UQuestInfoSaveData>();
					QuestInfoSaveData->QuestSaveData = LoadData->QuestSaveData;
					Saveable->LoadData(*QuestInfoSaveData);
				}
				else if (SaveDataType == ESaveDataType::CharacterStatus)
				{
					UCharacterStatusSaveData* CharacterSaveData = NewObject<UCharacterStatusSaveData>();
					CharacterSaveData->CurrentHealth = LoadData->CurrentHealthSaveData;
					Saveable->LoadData(*CharacterSaveData);
				}
			}
		}
		
		if (bFlagLoadCharacterLocation)
		{
			auto asd = GetWorld()->GetFirstPlayerController();
			auto asd123 = GetWorld()->GetFirstPlayerController()->GetPawn();
			GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(LoadData->Location);
		}
	}
}

void USaveManager::LoadLevelData()
{
	USaveGameData* LoadData = Cast<USaveGameData>(UGameplayStatics::LoadGameFromSlot(TEXT("PlayerSaveSlot"), 0));
	
	if (LoadData->WorldName.IsValid())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LoadData->WorldName, false);	
		bFlagLoadCharacterLocation = true;
	}
	else
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), TitleLevel);
	}
}

void USaveManager::SaveGameData()
{
	USaveGameData* PlayerSaveData = Cast<USaveGameData>(UGameplayStatics::CreateSaveGameObject(USaveGameData::StaticClass()));
	
	for (auto& Component : RegisteredComponents)
	{
		if (ISaveableInterface* Saveable = Cast<ISaveableInterface>(Component.Value))
		{
			USaveGameDataBase* SaveDataFragment = nullptr;
			Saveable->SaveData(SaveDataFragment);
			ESaveDataType SaveDataType = Saveable->SaveDataType();
			
			if (SaveDataType == ESaveDataType::Inventory)
			{
				if (UInventorySaveData* InventorySaveData = Cast<UInventorySaveData>(SaveDataFragment))
				{
					PlayerSaveData->InventorySaveData = InventorySaveData->InventoryItemList;	
				}
			}
			else if (SaveDataType == ESaveDataType::AbilitySlot)
			{
				if (UAbilitySlotSaveData* AbilitySaveData = Cast<UAbilitySlotSaveData>(SaveDataFragment))
				{
					PlayerSaveData->AbilityClassSaveData = AbilitySaveData->AbilitySlotClassList;	
				}
			}
			else if (SaveDataType == ESaveDataType::QuickSlot)
			{
				if (UQuickSlotSaveData* QuickSlotSaveData = Cast<UQuickSlotSaveData>(SaveDataFragment))
				{
					PlayerSaveData->QuickSlotNameSaveData = QuickSlotSaveData->QuickSlotItemName;	
				}
			}
			else if (SaveDataType == ESaveDataType::Quest)
			{
				if (UQuestInfoSaveData* QuestSaveData = Cast<UQuestInfoSaveData>(SaveDataFragment))
				{
					PlayerSaveData->QuestSaveData = QuestSaveData->QuestSaveData;	
				}
			}
			else if (SaveDataType == ESaveDataType::CharacterStatus)
			{
				if (UCharacterStatusSaveData* CharacterSaveData = Cast<UCharacterStatusSaveData>(SaveDataFragment))
				{
					PlayerSaveData->CurrentHealthSaveData = CharacterSaveData->CurrentHealth;
				}
			}
		}
	}
	
	PlayerSaveData->WorldName = *GetWorld()->GetName();
	PlayerSaveData->Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	
	UGameplayStatics::SaveGameToSlot(PlayerSaveData, TEXT("PlayerSaveSlot"), 0);
}
