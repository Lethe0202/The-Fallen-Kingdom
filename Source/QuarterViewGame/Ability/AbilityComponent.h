// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

class UBaseAbility;
class UActionComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUAbilityComponentInitialized);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAbilityComponent();

	virtual void ActivateAbility(const int32 index);
	virtual void ActivateAbilityFromClass(TSubclassOf<UBaseAbility> AbilityClass);
	void ApplyAbilityEffect(TSubclassOf<UBaseAbility> AbilityClass, const TArray<AActor*>& Targets);
	void ApplyAbilityEffect(TSubclassOf<UBaseAbility> AbilityClass, const TArray<AActor*>& Targets, const TArray<FVector>& HitNormal);
	void AbilityAdditionalAction(TSubclassOf<UBaseAbility> AbilityClass, int32 ActionIndex);

	virtual void ClearTempData();
	
protected:
	virtual void InitializeComponent() override;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	void SetEnableComboInput(bool bEnable) { bEnableComboInput = bEnable; }
	void SetInputNextCombo(bool bNextCombo) { bInputNextCombo = bNextCombo; }
	const bool GetInputNextCombo() const { return bInputNextCombo; }

protected:
	UFUNCTION()
	void HandleAbilityStarted();

	UFUNCTION()
	void HandleAbilityEnded();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UBaseAbility>> StartAbilityClassList;

	UPROPERTY()
	TArray<TObjectPtr<UBaseAbility>> AbilityList;

	UPROPERTY()
	UActionComponent* ActionComponent;

	bool bEnableComboInput = false;
	bool bInputNextCombo = false;
	int32 CurrentWaitingAbilityIndex = -1;
};
