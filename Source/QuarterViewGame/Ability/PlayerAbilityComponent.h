// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityComponent.h"
#include "InputActionValue.h"
#include "../Common/AbilityStructs.h"
#include "QuarterViewGame/Interface/SaveableInterface.h"
#include "PlayerAbilityComponent.generated.h"

class UEnhancedInputComponent;
class UInputMappingContext;
class UInputAction;
class ADecalActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilitySlotUpdate, const UInputAction*, InputAction, UPlayerAbilityComponent*, AbilityComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnUpdateAbilityProgressSignature, float, Percent, float, HoldTime, float, MinPerfectZoneTime, float, MaxPerfectZoneTime);

UCLASS()
class QUARTERVIEWGAME_API UPlayerAbilityComponent : public UAbilityComponent, public ISaveableInterface
{
	GENERATED_BODY()
	
public:
	UPlayerAbilityComponent();

	void InitPlayerInputComponent(UInputComponent* PlayerInputComponent);
	
	virtual void ActivateAbility(const int32 index) override;
	
	UFUNCTION()
	void HandleAbilitySlotChange(UInputAction* Slot1, UInputAction* Slot2);

	UFUNCTION()
	void HandleRegisterAbilitySlot(UInputAction* Slot, const FString AbilityID);

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Input_AbilityStarted(const FInputActionValue& Value, const int32 index);
	void Input_AbilityTriggered(const FInputActionValue& Value, const int32 index);
	void Input_AbilityInputReleased(const FInputActionValue& Value, const int32 index);
	void Input_LeftMouseClick(const FInputActionValue& Value);

	void Input_DashStarted(const FInputActionValue& Value);

	void InputReleased(const int32 index);
	void HoldAbilityTriggered(const int32 index, const FAbilityStaticData& AbilityStaticData, FAbilityRuntimeData& AbilityRumtimeData);
	void CheckNextCombo();
	void ToggleSpotDecal(const int32 index);
	
private:
	void UpdateAbilityDecalLocation();
	void RemoveAbilitySlot(UInputAction* InputActionSlot);
	void RebindInputAction(int32 AbilityIndex);
	
public:
	UBaseAbility* GetAbilityFromInputAction(const UInputAction* InputAction) const;
	const FString GetAbilityInputKeyFromInputAction(const UInputAction* InputAction) const;

public:
	/* ISaveableInterface */
	virtual void LoadData(USaveGameDataBase& LoadData) override;
	virtual void SaveData(/*out*/USaveGameDataBase*& SaveData) override;
	virtual ESaveDataType SaveDataType() override;
	
public:
	FOnUpdateAbilityProgressSignature OnUpdateAbilityProgress;
	FOnAbilitySlotUpdate OnAbilitySlotUpdate;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> AbilityInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UInputAction>> AbilityInputActionList;

	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ActivateWaitingAbilityInputAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> DashInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseAbility> AbilityDashClass;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ADecalActor> AbilityDecalActorClass;

private:
	TObjectPtr<UEnhancedInputComponent> PlayerEnhancedInputComponent;
	uint32 MouseClickHandle;

	TObjectPtr<UBaseAbility> AbilityDash;

	TObjectPtr<ADecalActor> AbilityDecalActor;
	TWeakObjectPtr<UDecalComponent> AbilityDecalComponent;
};
