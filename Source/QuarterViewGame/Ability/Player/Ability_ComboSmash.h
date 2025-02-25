// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_ComboSmash.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbility_ComboSmash : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;
	virtual void AdditionalAction(int32 ActionIndex) override;
};
