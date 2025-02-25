// Fill out your copyright notice in the Description page of Project Settings.


#include "QuarterViewGameInstance.h"

#include "ItemManager.h"
#include "SaveManager.h"
#include "../Manager/AnimationManager.h"
#include "../Manager/DataTableManager.h"
#include "../Manager/UIManager.h"
#include "../Manager/QuestManager.h"
#include "../Manager/ItemManager.h"
#include "../Manager/WorldMapManager.h"
#include "../Manager/QVCheatManager.h"
#include "../Manager/QVEventManager.h"
#include "Blueprint/UserWidget.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/LoadingScreenUserWidget.h"

void UQuarterViewGameInstance::Init()
{
	Super::Init();
	
	if (AnimationManagerClass != nullptr)
	{
		AnimationManager = NewObject<UAnimationManager>(this, AnimationManagerClass);
	}	

	if (DataTableManagerClass != nullptr)
	{
		DataTableManager = NewObject<UDataTableManager>(this, DataTableManagerClass);
	}
	
	if (UIManagerClass != nullptr)
	{
		UIManager = NewObject<UUIManager>(this, UIManagerClass);
	}
	
	if (QuestManagerClass != nullptr)
	{
		QuestManager = NewObject<UQuestManager>(this, QuestManagerClass);
	}
	
	if (ItemManagerClass != nullptr)
	{
		ItemManager = NewObject<UItemManager>(this, ItemManagerClass);
	}

	if (WorldMapManagerClass != nullptr)
	{
		WorldMapManager = NewObject<UWorldMapManager>(this, WorldMapManagerClass);
	}

	if (EventManagerClass != nullptr)
	{
		EventManager = NewObject<UQVEventManager>(this, EventManagerClass);
	}
	
	if (SaveManagerClass != nullptr)
	{
		SaveManager = NewObject<USaveManager>(this, SaveManagerClass);
	}

	//월드내 액터가 초기화되고 액터 관련해서 필요한 정보를 수집하기 위한 delegate 등록
	FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &UQuarterViewGameInstance::OnWorldInitializedActors);
	
	//레벨 전환 시 로딩 스크린을 위한 Delegate
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UQuarterViewGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UQuarterViewGameInstance::OnPostLoadMap);
}

void UQuarterViewGameInstance::ChangeLevel(const TSoftObjectPtr<UWorld> NewLevel, FString Options)
{
	if (SaveManager)
	{
		SaveManager->SaveGameData();
	}
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(),NewLevel, true, Options);
}

void UQuarterViewGameInstance::OnWorldInitializedActors(const FActorsInitializedParams& Params)
{
	QuestManager->LoadNPCQuestFromCurrentWorld();
	WorldMapManager->InitWorldMap();
}

void UQuarterViewGameInstance::OnPreLoadMap(const FString& MapName)
{
	LoadingWidget = CreateWidget(this, LoadingWidgetClass);
	
	UGameViewportClient* GameViewportClient = GetGameViewportClient();
	GameViewportClient->AddViewportWidgetContent(LoadingWidget->TakeWidget(), 200);
	
	FSlateApplication::Get().Tick();
}

void UQuarterViewGameInstance::OnPostLoadMap(UWorld* LoadedWorld)
{
	SaveManager->LoadGameData();
	
	if (UGameViewportClient* GameViewportClient = GetGameViewportClient())
	{
		Cast<ULoadingScreenUserWidget>(LoadingWidget)->PlayAnim();
	}
}

UAnimationManager* UQuarterViewGameInstance::GetAnimManager() const
{
	return AnimationManager ? AnimationManager : nullptr;
}

UDataTableManager* UQuarterViewGameInstance::GetDataTableManager() const
{
	return DataTableManager ? DataTableManager : nullptr;
}

UUIManager* UQuarterViewGameInstance::GetUIManager() const
{
	return UIManager ? UIManager : nullptr;
}

UQuestManager* UQuarterViewGameInstance::GetQuestManager() const
{
	return QuestManager ? QuestManager : nullptr;
}

UItemManager* UQuarterViewGameInstance::GetItemManager() const
{
	return ItemManager ? ItemManager : nullptr;
}

UWorldMapManager* UQuarterViewGameInstance::GetWorldMapManager() const
{
	return WorldMapManager ? WorldMapManager : nullptr;
}

UQVEventManager* UQuarterViewGameInstance::GetEventManager() const
{
	return EventManager ? EventManager : nullptr;
}

USaveManager* UQuarterViewGameInstance::GetSaveManager() const
{
	return SaveManager ? SaveManager : nullptr;
}
