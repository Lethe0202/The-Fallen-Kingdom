// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestObjectiveTextBlock.h"

#include "Components/TextBlock.h"

void UQuestObjectiveTextBlock::InitObjectiveTextBlock(const FText& Text, bool bSuccess)
{
	TextBlock->SetText(Text);
}
