// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_FocusTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_FocusTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bFocus)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorKeySelector.SelectedKeyName);
		
		if (AActor* TargetActor = Cast<AActor>(TargetObject))
		{
			OwnerComp.GetAIOwner()->SetFocus(TargetActor, EAIFocusPriority::Gameplay);
		}
	}
	else
	{
		OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
