// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDBossHealthBarWidget.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDBossHealthBarWidget::NativeInit(UCharacterStatusComponent* CharacterStatusComponent)
{
	Super::NativeInit(CharacterStatusComponent);
}

void UHUDBossHealthBarWidget::SetBossDisplayName(const FText& InBossDisplayName)
{
	BossDisplayName_TextBlock->SetText(InBossDisplayName);
}

void UHUDBossHealthBarWidget::SetHealthBarNum(int32 InHealthBarNum)
{
	HealthBarNum = InHealthBarNum;
	CurrentHealthBarNum = InHealthBarNum;
	
	if (InHealthBarNum >= 1)
	{
		FString HealthBarCntString = "x" + FString::FromInt(CurrentHealthBarNum);
		FText HealthBarCnt = FText::FromString(HealthBarCntString);
		
		HealthBarCnt_TextBlock->SetText(HealthBarCnt);
	}
	else
	{
		HealthBarCnt_TextBlock->SetText(FText());
		NextHealth_Image->SetColorAndOpacity(FColor(1.f, 1.f, 1.f, 0.5f));
	}
}

void UHUDBossHealthBarWidget::UpdateHealthBar(float NewPercent)
{
	float FillPercent = (NewPercent * HealthBarNum) - (CurrentHealthBarNum - 1);
	
	Super::UpdateHealthBar(FillPercent);
	
	if (FillPercent <= 0.f)
	{
		--CurrentHealthBarNum;
		HealthProgressBar->SetFillColorAndOpacity(ColorArray[CurrentHealthBarNum % ColorArray.Num()]);
		
		FString HealthBarCntString = "x" + FString::FromInt(CurrentHealthBarNum);
		FText HealthBarCnt = FText::FromString(HealthBarCntString);
		
		HealthBarCnt_TextBlock->SetText(HealthBarCnt);
		
		if (CurrentHealthBarNum > 1)
		{
			NextHealth_Image->SetColorAndOpacity(ColorArray[(CurrentHealthBarNum - 1) % ColorArray.Num()]);
		}
		else
		{
			NextHealth_Image->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
		}
	}
}
