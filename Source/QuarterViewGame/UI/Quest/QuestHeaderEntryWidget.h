// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "QuestHeaderEntryWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class QUARTERVIEWGAME_API UQuestHeaderEntryWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	//퀘스트창에서 수락, 제거 등 중앙하단 버튼 클릭
	UFUNCTION()
	void OnButtonClicked();
	
public:
	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(Meta = (BindWidget))
	TObjectPtr<UTextBlock> QuestDisplayName;
	
private:
	FName QuestID;
};
