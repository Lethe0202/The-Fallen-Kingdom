// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class AAIBossCharacter;
class UHUDBossHealthBarWidget;
class UCharacterStatusComponent;
class UInventoryPanelWidget;
class UInventoryComponent;
class UQuestTabWidget;

UCLASS()
class QUARTERVIEWGAME_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void InitAbilitySlot(UPlayerAbilityComponent* AbilityComponent);

	UFUNCTION(BlueprintNativeEvent)
	void InitItemQuickSlot(UItemQuickSlotComponent* ItemQuickSlotComponent);
	
	UFUNCTION(BlueprintNativeEvent)
	void InitHealthBar(UCharacterStatusComponent* CharacterStatusComponent);

	void InitInventoryPanel(UInventoryComponent* InventoryComponent);
	
public:
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UFUNCTION(BlueprintCallable)
	void ToggleAbilityPanel();

	UFUNCTION(BlueprintCallable)
	void ToggleQuestPanel();
	
	UFUNCTION(BlueprintCallable)
	void ToggleWorldMap();
	
	void InitTopBossHpBar(AAIBossCharacter* AIBossCharacter);
	void RemoveTopBossHpBar();
	const bool IsVisibleBossHPBar(const FName& BossID) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInventoryPanelWidget> InventoryPanel;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> AbilityPanel;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestTabWidget> QuestTabWidgetClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UQuestTabWidget> QuestTab;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> WorldMapWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UUserWidget> WorldMapWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UHUDBossHealthBarWidget> TopBossHealthBarClass;

	UPROPERTY()
	TObjectPtr<UHUDBossHealthBarWidget> TopBossHealthBar;
};
