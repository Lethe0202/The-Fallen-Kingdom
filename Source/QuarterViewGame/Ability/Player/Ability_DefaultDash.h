// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_DefaultDash.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbility_DefaultDash : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	
protected:
	UFUNCTION()
	void OnChangeCollisionChannle();
};
