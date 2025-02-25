// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PrimaryAttack.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"

#include "../Character/AICharacter.h"

EBTNodeResult::Type UBTT_PrimaryAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICharacter* AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn())->PrimaryAttack();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
