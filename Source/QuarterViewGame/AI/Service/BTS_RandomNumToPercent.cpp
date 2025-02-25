// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_RandomNumToPercent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBTS_RandomNumToPercent::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
	
	float RandomPercent = UKismetMathLibrary::RandomFloatInRange(0, 1);

	for (int i = 0; i < Percents.Num(); ++i)
	{
		if (Percents[i] >= RandomPercent)
		{
			UBlackboardComponent* BlackboardComp = SearchData.OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsInt(RandomNumKeySelector.SelectedKeyName, i);
			break;
		}
	}
}
