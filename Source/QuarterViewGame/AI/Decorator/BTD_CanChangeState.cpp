// Fill out your copyright notice in the Description page of Project Settings.


#include "BTD_CanChangeState.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"
#include "../Character/AICharacter.h"
#include "../Character/Component/ActionComponent.h"

bool UBTD_CanChangeState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (AAICharacter* AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (!Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn())->GetActionComponent())
		{
			return false;
		}
	
		bool bCanChangeState = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn())->GetActionComponent()->CanChangeState(CharacterStateType);
		
		return bCanChangeState;
	}

	return false;
}
