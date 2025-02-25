// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDHealthBarWidget.h"
#include "HUDBossHealthBarWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UHUDBossHealthBarWidget : public UHUDHealthBarWidget
{
	GENERATED_BODY()

public:
	virtual void NativeInit(UCharacterStatusComponent* CharacterStatusComponent) override;
	
	void SetBossDisplayName(const FText& InBossDisplayName);
	void SetHealthBarNum(int32 InHealthBarNum);
	
protected:
	virtual void UpdateHealthBar(float NewPercent) override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> BossDisplayName_TextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthBarCnt_TextBlock;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> NextHealth_Image;
	
	int32 HealthBarNum = -1;
	int32 CurrentHealthBarNum = -1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TArray<FLinearColor> ColorArray;
};
