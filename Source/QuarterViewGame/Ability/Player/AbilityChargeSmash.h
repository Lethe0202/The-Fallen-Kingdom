// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityFunctionLibrary.h"
#include "BaseAbility.h"
#include "AbilityChargeSmash.generated.h"

/**
 * 
 */

//Hold Ability
UCLASS()
class QUARTERVIEWGAME_API UAbilityChargeSmash : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;
	virtual void ReleasedInput() override;
};
