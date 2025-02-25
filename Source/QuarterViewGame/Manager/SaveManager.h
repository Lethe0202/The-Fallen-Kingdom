// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/SaveGame.h"
#include "SaveManager.generated.h"

struct FInventoryItem;
class UBaseAbility;

UENUM()
enum class ESaveDataType : uint8
{
	Inventory,
	AbilitySlot,
	QuickSlot,
	CharacterStatus,
	Quest,
};

UCLASS(BlueprintType)
class USaveGameDataBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual ~USaveGameDataBase() = default;
};

UCLASS(BlueprintType)
class UInventorySaveData : public USaveGameDataBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FInventoryItem> InventoryItemList;
};

UCLASS(BlueprintType)
class UAbilitySlotSaveData : public USaveGameDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<TSubclassOf<UBaseAbility>> AbilitySlotClassList;
};

UCLASS(BlueprintType)
class UQuickSlotSaveData : public USaveGameDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FName> QuickSlotItemName;
};

USTRUCT()
struct FInProgressQuestSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName QuestID;

	UPROPERTY()
	bool bCompletion;
	
	UPROPERTY()
	int32 CurrentTaskIndex;

	UPROPERTY()
	TArray<bool> bTaskObjectiveCompleted;
	
	UPROPERTY()
	TArray<int32> TaskObjectiveQuantity;
};

USTRUCT()
struct FQuestSaveData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInProgressQuestSaveData> InProgressQuestNameArray;
	
	UPROPERTY()
	TArray<FName> SuccessQuestArray;
};

UCLASS(BlueprintType)
class UQuestInfoSaveData : public USaveGameDataBase
{
	GENERATED_BODY()
	
public:
	FQuestSaveData QuestSaveData;
};

UCLASS(BlueprintType)
class UCharacterStatusSaveData : public USaveGameDataBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int32 CurrentHealth;
};


UCLASS()
class USaveGameData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FInventoryItem> InventorySaveData;
	
	UPROPERTY()
	TArray<TSubclassOf<UBaseAbility>> AbilityClassSaveData;
	
	UPROPERTY()
	TArray<FName> QuickSlotNameSaveData;

	UPROPERTY()
	FQuestSaveData QuestSaveData;

	UPROPERTY()
	int32 CurrentHealthSaveData;
	
	UPROPERTY()
	FName WorldName;
	
	UPROPERTY()
	FVector Location;
};

UCLASS(Blueprintable)
class QUARTERVIEWGAME_API USaveManager : public UObject
{
	GENERATED_BODY()
	
public:
	void InitSaveManager(APlayerController* PlayerController);
	
	void LoadGameData();

	UFUNCTION(BlueprintCallable)
	void LoadLevelData();
	
	void SaveGameData();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> TitleLevel;
	
	TMap<ESaveDataType, UObject*> RegisteredComponents;
	bool bFlagLoadCharacterLocation = false;
};
