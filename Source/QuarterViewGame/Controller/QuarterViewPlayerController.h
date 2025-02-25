// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "QuarterViewPlayerController.generated.h"

class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

class APlayerCharacter;
class UPlayerHUD;
class UActionComponent;

UCLASS(BlueprintType)
class QUARTERVIEWGAME_API AQuarterViewPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AQuarterViewPlayerController();
	
	UPlayerHUD* GetPlayerHUD() const;
	
protected:
	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	void OnSetDestinationStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	
protected:
	void Input_OpenInventory(const FInputActionValue& Value);
	void Input_OpenAbilityPanel(const FInputActionValue& Value);
	void Input_Interaction(const FInputActionValue& Value);
	void Input_QuestTab(const FInputActionValue& Value);
	void Input_OpenWorldMap(const FInputActionValue& Value);

	bool IsCursorOnUI();
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	float ShortPressThreshold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* FXCursor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenAbilityPanelAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenQuestTabAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenWorldMpaAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUDWidget, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UPlayerHUD> HUDWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUDWidget, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPlayerHUD> PlayerHUD;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> OwnCharacter;

	UPROPERTY()
	TWeakObjectPtr<UActionComponent> OwnerActionComponent;

private:
	FVector CachedDestination;

	float FollowTime;
};
