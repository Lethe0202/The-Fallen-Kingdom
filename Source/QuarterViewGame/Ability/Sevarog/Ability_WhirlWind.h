// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_WhirlWind.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbility_WhirlWind : public UBaseAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;
	virtual void AdditionalAction(int32 ActionIndex) override;

protected:
	UFUNCTION()
	void OnMovementTimer();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed;

private:
	FTimerHandle MovementTimerhandle;
	FVector MovementDirectionVector;
};
