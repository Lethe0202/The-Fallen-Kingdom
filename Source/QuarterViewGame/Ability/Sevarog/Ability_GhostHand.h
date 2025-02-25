// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityFunctionLibrary.h"
#include "BaseAbility.h"
#include "Ability_GhostHand.generated.h"

class AAbilityEffectHandleActor;

UCLASS()
class QUARTERVIEWGAME_API UAbility_GhostHand : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;

	UFUNCTION()
	virtual void AbilityEffect(AActor* EffectTarget) override;

protected:
	UFUNCTION()
	void OnGhostHandEffect(AActor* TargetActor);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	TSubclassOf<AAbilityEffectHandleActor> GhostHandActorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	int32 SpawnCount;
};
