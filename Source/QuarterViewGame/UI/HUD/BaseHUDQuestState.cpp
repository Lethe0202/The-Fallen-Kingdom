// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUDQuestState.h"

#include "Components/TextBlock.h"

void UBaseHUDQuestState::SetDescription(FText InDescription)
{
	Description_TextBlock->SetText(InDescription);
}
