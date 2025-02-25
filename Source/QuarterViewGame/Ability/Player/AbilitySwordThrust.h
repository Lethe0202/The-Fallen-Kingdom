// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbility.h"
#include "AbilitySwordThrust.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbilitySwordThrust : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility() override;
	virtual void AbilityEffect(AActor* EffectTarget) override;
	virtual void AdditionalAction(int32 ActionIndex) override;

protected:
	virtual void StartAbilityCoolingTimer() override;
	virtual void ResetRumtimeData() override;

	UFUNCTION()
	void OnRotationTimer();

	UFUNCTION()
	void OnDashTimer();

	UFUNCTION()
	void OnChangeCollisionChannle();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivateAccess = "true"))
	float RotationTime;

	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivateAccess = "true"))
	float DashDistance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivateAccess = "true"))
	float DashTime;
		

	//TSoftClassPtr<UParticleSystem>
	UPROPERTY(EditDefaultsOnly, Category = "Ability | Detail", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* StartLocationFX;
	
	UPROPERTY()
	UParticleSystemComponent* StartLocationFXComponent;

	//1
	FRotator TargetRotation;
	FRotator StartRotation;
	float RotateStartTime = 0.f;
	FTimerHandle RotationTimerHandle;

	//2
	FVector DashStartLocation;
	FVector DashTargetLocation;
	float DashStartTime = 0.f;
	FTimerHandle DashTimerHandle;
};
