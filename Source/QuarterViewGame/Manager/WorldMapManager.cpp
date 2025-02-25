// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMapManager.h"
#include "AIBossCharacter.h"
#include "../Quest/QuestGiverComponent.h"
#include "QuarterViewGameInstance.h"
#include "QuestManager.h"

#include "EngineUtils.h"

void UWorldMapManager::InitWorldMap()
{
	WorldMapItems.Empty();
	QuestMapItems.Empty();
	
	LoadMapIconActorFromCurrentWorld();
	
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UQuestManager* QuestManager = QuarterViewGameInstance->GetQuestManager())
		{
			const TArray<UQuestGiverComponent*> AllQuestGiverComponents = QuestManager->GetWorldAllQuestGiverComponents();
			
			for (auto& QuestGiverComponent : AllQuestGiverComponents)
			{
				FVector Location = QuestGiverComponent->GetOwner()->GetActorLocation();
				FVector2D Location2D = FVector2D(Location.X, Location.Y);
				
				FMapItem MapItem;
				MapItem.OwnerUniqueID = QuestGiverComponent->GetOwner()->GetUniqueID();
				MapItem.Location = Location2D;
				MapItem.Icon = QuestGiverComponent->GetCurrentStateIcon();
				MapItem.Color = QuestGiverComponent->GetCurrentStateIconColor();
				
				QuestMapItems.Add(MapItem.OwnerUniqueID, MapItem);
			}
		}
	}
	
	CurrentWorldInfoTableRow = WorldInfoDataTable->FindRow<FWorldInfoTableRow>(FName(GetWorld()->GetName()), TEXT(""));
}

void UWorldMapManager::AddNewMapIconItem(AActor* TargetActor)
{
	if (TargetActor->Implements<UMapIconInterface>())
	{
		FVector ActorLocation = TargetActor->GetActorLocation();
		EMapIconType MapIconType = IMapIconInterface::Execute_GetMapIconType(TargetActor);
		
		FMapItem MapItem;
		MapItem.OwnerUniqueID = TargetActor->GetUniqueID();
		MapItem.Location = FVector2D(ActorLocation.X, ActorLocation.Y);
		MapItem.Icon = IconTextureMaps[MapIconType];
		MapItem.MapIconMovableType = IMapIconInterface::Execute_GetMapIconMovableType(TargetActor);
		MapItem.Color = FColor::White;
		
		if (MapItem.MapIconMovableType == EMapIconMovableType::Dynamic)
		{
			MapItem.Owner = TargetActor;
		}
		
		WorldMapItems.Add(MapItem.OwnerUniqueID, MapItem);
		OnWorldMapIconChanged.Broadcast(MapItem.OwnerUniqueID, WorldMapItems[MapItem.OwnerUniqueID].Icon, FColor::White);
	}
}

void UWorldMapManager::RemoveMapIconItem(int32 OwnerUniqueID)
{
	if (WorldMapItems.Contains(OwnerUniqueID))
	{
		WorldMapItems.Remove(OwnerUniqueID);
		OnWorldMapIconChanged.Broadcast(OwnerUniqueID, nullptr, FColor::White);
	}
}

void UWorldMapManager::ChangeMapQuestIcon(int32 OwnerUniqueID, const TSoftObjectPtr<UTexture2D>& Icon, const FColor& ColorAndOpacity)
{
	if (Icon == nullptr)
	{
		//삭제
		QuestMapItems[OwnerUniqueID].Icon = nullptr;
		OnWorldMapIconChanged.Broadcast(OwnerUniqueID, WorldMapItems[OwnerUniqueID].Icon, FColor::White);
	}
	else
	{
		//변경
		QuestMapItems[OwnerUniqueID].Icon = Icon;
		QuestMapItems[OwnerUniqueID].Color = ColorAndOpacity;
		OnWorldMapIconChanged.Broadcast(OwnerUniqueID, Icon, ColorAndOpacity);
	}
}

const TArray<FMapItem> UWorldMapManager::GetAllMapItems() const
{
	TMap<int32, FMapItem> AllMapItemsMap;
	AllMapItemsMap.Append(WorldMapItems);
	
	for (auto& QuestMapItem : QuestMapItems)
	{
		if (QuestMapItem.Value.Icon)
		{
			AllMapItemsMap.Add(QuestMapItem);	
		}
	}
	
	TArray<FMapItem> AllMapItems;
	AllMapItemsMap.GenerateValueArray(AllMapItems);
	
	return AllMapItems;
}

const FMapItem* UWorldMapManager::GetMapItem(int32 OwnerUniqueID) const
{
	if (WorldMapItems.Contains(OwnerUniqueID))
	{
		return &WorldMapItems[OwnerUniqueID];
	}

	return nullptr;
}

const float UWorldMapManager::GetCurrentWorldOrthoWidth() const
{
	if (CurrentWorldInfoTableRow)
	{
		return CurrentWorldInfoTableRow->OrthoWidth;
	}

	return 0.f;
}

UTexture2D* UWorldMapManager::GetCurrentWorldMapTexture() const
{
	if (CurrentWorldInfoTableRow)
	{
		return CurrentWorldInfoTableRow->WorldMapTexture2D.LoadSynchronous();
	}

	return nullptr;;
}

void UWorldMapManager::LoadMapIconActorFromCurrentWorld()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor)
		{
			continue;
		}
		
		if (Actor->Implements<UMapIconInterface>())
		{
			FVector ActorLocation = Actor->GetActorLocation();
			EMapIconType MapIconType = IMapIconInterface::Execute_GetMapIconType(Actor);
			
			FMapItem MapItem;
			MapItem.OwnerUniqueID = Actor->GetUniqueID();
			MapItem.Location = FVector2D(ActorLocation.X, ActorLocation.Y);
			MapItem.Icon = IconTextureMaps[MapIconType];
			MapItem.MapIconMovableType = IMapIconInterface::Execute_GetMapIconMovableType(Actor);
			MapItem.Color = FColor::White;
		
			if (MapItem.MapIconMovableType == EMapIconMovableType::Dynamic)
			{
				MapItem.Owner = Actor;
			}
				
			WorldMapItems.Add(MapItem.OwnerUniqueID, MapItem);
		}
	}
}
