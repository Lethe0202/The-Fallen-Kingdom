// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Common/QVEnumHeader.h"

#include "UIManager.generated.h"

class UBaseHUDQuestState;
class UPlayerHUD;

UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UUIManager : public UObject
{
	GENERATED_BODY()
	
public:
	//퀘스트 수락했을때 UI 생성
	void QuestAccept(const FText& QuestDisplayName);

	//퀘스트 목표 컨디션 업데이트됐을때 UI 생성
	void QuestObjectiveConditionUpdate(const FText& TaskConditionText);

	//퀘스트 목표 완료했을때 UI 생성
	void QuestObjectiveComplete(const FText& TaskConditionDisplayText);

	//퀘스트 성공했을때 UI 생성
	void QuestSuccess(const FText& QuestDisplayName);
	
	UTexture2D* GetAbilityActivationTypeIcon(EAbilityActivationType AbilityActivationType) const;

	//플레이어 초기화 후, UIManager에 등록
	void RegisterPlayerHUD(UPlayerHUD* HUD);

	//플레이어 사망 시, 호출
	void ViewDeadScreen();
	
	const TWeakObjectPtr<UPlayerHUD> GetPlayerHUD() const;
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TMap<EAbilityActivationType, TSoftObjectPtr<UTexture2D>> AbilityActivationTypeIcon;

	UPROPERTY()
	TWeakObjectPtr<UPlayerHUD> PlayerHUD;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> DeadScreenWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseHUDQuestState> QuestAcceptWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseHUDQuestState> QuestSuccessWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UBaseHUDQuestState> QuestConditionWidgetClass;

	UPROPERTY()
	TObjectPtr<UBaseHUDQuestState> QuestConditionClass;
};
