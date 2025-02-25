// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemEffectBase.generated.h"

class ACharacter;

/**
 * 사용가능한 아이템의 효과 Base클래스
 */

UCLASS(BlueprintType)
class QUARTERVIEWGAME_API UItemEffectBase : public UDataAsset
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(ACharacter* TargetCharacter){};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolingTime;
};
