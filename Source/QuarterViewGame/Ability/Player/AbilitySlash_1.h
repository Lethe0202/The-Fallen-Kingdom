// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "AbilitySlash_1.generated.h"

class AAbilityEffectHandleActor;

UCLASS()
class QUARTERVIEWGAME_API UAbilitySlash_1 : public UBaseAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;
	virtual void AdditionalAction(int32 ActionIndex) override;

protected:
	UFUNCTION()
	void OnDoTHandle(AActor* Target);

	UFUNCTION()
	void OnDoTDestroy();


private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	TSubclassOf<AAbilityEffectHandleActor> DoTActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	FVector SpawnOffSet;

	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivaetAccess = "true"))
	float ActorLifeTime = 0.1f;

private:
	//Damage of time
	UPROPERTY()
	TObjectPtr<AAbilityEffectHandleActor> AbilityEffectHandleActor;
	FTimerHandle DoTDestroyTimerHandle;
};
