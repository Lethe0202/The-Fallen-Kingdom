// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Ability_DataAsset.generated.h"

/**
 * 단순히 각 캐릭터가 가지고 있는 스킬ID
 */
UCLASS(BlueprintType)
class QUARTERVIEWGAME_API UAbility_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FString> AbilityID;
};
