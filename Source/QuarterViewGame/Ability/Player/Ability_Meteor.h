// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "Ability_Meteor.generated.h"

class AAbilityEffectHandleActor;

UCLASS()
class QUARTERVIEWGAME_API UAbility_Meteor : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;

protected:
	UFUNCTION()
	void OnMeteorEffect(const FVector& Location);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAbilityEffectHandleActor> MeteorFXActorClass;
};
