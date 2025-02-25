// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_TryActivateAbility.h"
#include "../Character/AICharacter.h"
#include "../Ability/AbilityComponent.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AAICharacter* AICharacter = Cast<AAICharacter>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (UAbilityComponent* AbilityComponent = AICharacter->GetAbilityComponent())
		{
			AbilityComponent->ActivateAbilityFromClass(AbilityClass);
		}

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
