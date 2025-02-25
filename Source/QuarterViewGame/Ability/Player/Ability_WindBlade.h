// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "../AbilityFunctionLibrary.h"
#include "Ability_WindBlade.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbility_WindBlade : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	FKnockBackEffectType KnockBackEffectType;
};
