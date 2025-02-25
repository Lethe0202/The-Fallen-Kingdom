// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffectBase.h"
#include "HealingItemEffect.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UHealingItemEffect : public UItemEffectBase
{
	GENERATED_BODY()
	
public:
	virtual void ApplyEffect(ACharacter* TargetCharacter) override;

protected:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HealAmount;
};
