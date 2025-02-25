// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "QuarterViewGameInstance.generated.h"

class USaveManager;
class UAnimationManager;
class UDataTableManager;
class UDebugManager;
class UUIManager;
class UQuestManager;
class UItemManager;
class UWorldMapManager;
class UUserWidget;

UCLASS()
class QUARTERVIEWGAME_API UQuarterViewGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ChangeLevel(const TSoftObjectPtr<UWorld> NewLevel, FString Options);
	
protected:
	virtual void Init() override;
	
	void OnWorldInitializedActors(const FActorsInitializedParams& Params);
	
	UFUNCTION()
	void OnPreLoadMap(const FString& MapName);
	
	UFUNCTION()
	void OnPostLoadMap(UWorld* LoadedWorld);
	
public:
	UAnimationManager* GetAnimManager() const;
	UDataTableManager* GetDataTableManager() const;
	UUIManager* GetUIManager() const;

	UFUNCTION(BlueprintCallable)
	UQuestManager* GetQuestManager() const;
	
	UItemManager* GetItemManager() const;

	UFUNCTION(BlueprintCallable)
	UWorldMapManager* GetWorldMapManager() const;

	UFUNCTION(BlueprintCallable)
	UQVEventManager* GetEventManager() const;

	UFUNCTION(BlueprintCallable)
	USaveManager* GetSaveManager() const;
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDataTableManager> DataTableManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UAnimationManager> AnimationManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUIManager> UIManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQuestManager> QuestManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UItemManager> ItemManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UWorldMapManager> WorldMapManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UQVEventManager> EventManagerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<USaveManager> SaveManagerClass;
	
	UPROPERTY()
	TObjectPtr<UDataTableManager> DataTableManager;
	
	UPROPERTY()
	TObjectPtr<UAnimationManager> AnimationManager;

	UPROPERTY()
	TObjectPtr<UUIManager> UIManager;
	
	UPROPERTY()
	TObjectPtr<UQuestManager> QuestManager;
	
	UPROPERTY()
	TObjectPtr<UItemManager> ItemManager;
	
	UPROPERTY()
	TObjectPtr<UWorldMapManager> WorldMapManager;
	
	UPROPERTY()
	TObjectPtr<UQVEventManager> EventManager;

	UPROPERTY()
	TObjectPtr<USaveManager> SaveManager;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Manager", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> LoadingWidget;
};



