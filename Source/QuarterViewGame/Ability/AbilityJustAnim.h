// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "AbilityJustAnim.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbilityJustAnim : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail")
	bool bStagger = false;
};
