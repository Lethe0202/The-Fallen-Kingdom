// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuarterViewGameDataTable.h"
#include "UObject/NoExportTypes.h"
#include "WorldMapManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EMapIconType : uint8
{
	None,
	Boss,
	NPC,
	Portal,
	LandMark,
	QuestLocation,
};

/**
 * Static : 미니맵 아이콘 생성 시, 고정
 * Dynamic : 아이콘 생성 이후에도 계속 업데이트
 */
UENUM(BlueprintType)
enum class EMapIconMovableType : uint8
{
	Static,
	Dynamic,
};

class UTexture2D;

USTRUCT(BlueprintType)
struct FMapItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 OwnerUniqueID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector2D Location;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor Color;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMapIconMovableType MapIconMovableType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TWeakObjectPtr<AActor> Owner;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWorldMapIconChanged, int32, UniqueID, const TSoftObjectPtr<UTexture2D>&, Icon, const FColor&, ColorAndOpacity);

UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UWorldMapManager : public UObject
{
	GENERATED_BODY()
	
public:
	void InitWorldMap();

	//초기 초기화 이후에 새로운 MapIcon이 추가될때 호출 (퀘스트 위치 등)
	void AddNewMapIconItem(AActor* TargetActor);
	void RemoveMapIconItem(int32 OwnerUniqueID);

	//아이콘 상태(아이콘이미지 등)가 변경될떄 호출
	void ChangeMapQuestIcon(int32 OwnerUniqueID, const TSoftObjectPtr<UTexture2D>& Icon, const FColor& ColorAndOpacity = FColor::White);
	
	UFUNCTION(BlueprintCallable)
	const TArray<FMapItem> GetAllMapItems() const;

	const FMapItem* GetMapItem(int32 OwnerUniqueID) const;
	
	const float GetCurrentWorldOrthoWidth() const;
	UTexture2D* GetCurrentWorldMapTexture() const;

	//레벨 로드 후, 맵에 올라가야하는 아이콘 수집 
	void LoadMapIconActorFromCurrentWorld();
	
public:
	FOnWorldMapIconChanged OnWorldMapIconChanged;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EMapIconType, TSoftObjectPtr<UTexture2D>> IconTextureMaps;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDataTable> WorldInfoDataTable;
	
	//unique ID, MapItem (Static Icon)
	TMap<int32, FMapItem> WorldMapItems;

	//unique ID, MapItem (Static Icon)
	TMap<int32, FMapItem> QuestMapItems;
	
	FWorldInfoTableRow* CurrentWorldInfoTableRow;
};
