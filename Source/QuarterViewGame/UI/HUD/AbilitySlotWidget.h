// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Ability/AbilityComponent.h"
#include "../Slot/BaseSlotWidget.h"
#include "AbilitySlotWidget.generated.h"

class UImage;
class UTextBlock;

class UInputAction;

class UBaseAbility;
class UPlayerAbilityComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilitySlotChanged, UInputAction*, Slot1, UInputAction*, Slot2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilitySlotRegistered, UInputAction*, Slot, const FString, AbilityID);

UCLASS()
class QUARTERVIEWGAME_API UAbilitySlotWidget : public UBaseSlotWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Init(UPlayerAbilityComponent* AbilityComponent);
	
protected:
	void InitAbilityInputIcon(EAbilityActivationType AbilityActivationType);

protected:
	UFUNCTION()
	void OnUpdateAbilitySlot(const UInputAction* InputAction, UPlayerAbilityComponent* AbilityComponent);
	
	UFUNCTION()
	void OnUpdateAbilityCoolingTimer(float Percent, float RemainCoolingTime);

	UFUNCTION()
	void OnUpdateAbilityChainCoolingTimer(float Percent, float RemainCoolingTime);
	
public:
	UPROPERTY(meta = (BindWidget))
	UImage* AbilityInputIcon;

	UPROPERTY(meta = (BindWidget))
	UImage* AbilityCoolingGlow;

	UPROPERTY(meta = (BindWidget))
	UImage* AbilityChainCoolingGlow;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* AbilityButtonText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> CoolingCountText;

	TSubclassOf<UBaseAbility> AbilityClassRef;

	UPROPERTY(BlueprintCallable ,BlueprintAssignable)
	FOnAbilitySlotChanged OnAbilitySlotChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnAbilitySlotRegistered OnAbilitySlotRegistered;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	FName CoolingParameterName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	UInputAction* AbilityInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	bool bHiddenCoolTime = false;
};
