// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityFunctionLibrary.h"
#include "BaseAbility.h"
#include "../Utility/AbilityEffectHandleActor.h"
#include "Ability_SiphonExplosion.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbility_SiphonExplosion : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	
	UFUNCTION()
	virtual void AbilityEffect(AActor* EffectTarget) override;

protected:
	UFUNCTION()
	void OnSpawnExplosionTimer();

	UFUNCTION()
	void OnSiphonEffect(AActor* TargetActor);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	TSubclassOf<AAbilityEffectHandleActor> SiphonExplosionActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	int32 SpawnCount;

	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	float SpawnDelay;
	
private:
	FTimerHandle SpawnExplosionTimerHandle;
	int32 SpawnedCount = 0;
};
