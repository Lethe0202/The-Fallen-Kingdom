// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Quest/QuestGiverComponent.h"
#include "QuestStateWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UQuestStateWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ChangeState(EQuestState QuestState);
	
	void SetOwnerID(int32 InOwnerUniqueID);
	const int32 GetOwnerID() const;
	const UTexture* GetTexture() const;
	FLinearColor GetColor() const;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> ExclamationMarkIcon;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> QuestionMarkIcon;
	
private:
	int32 OwnerUniqueID;
};
