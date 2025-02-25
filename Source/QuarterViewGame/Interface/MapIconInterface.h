// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMapManager.h"
#include "UObject/Interface.h"
#include "MapIconInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMapIconInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 월드맵과 미니맵에 등록되어야하는 요소를 위한 인터페이스
 */
class QUARTERVIEWGAME_API IMapIconInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	EMapIconType GetMapIconType() const;

	UFUNCTION(BlueprintNativeEvent)
	EMapIconMovableType GetMapIconMovableType() const;
};
