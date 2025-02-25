// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "../../Character/State/ICharacterState.h"
#include "BTD_CanChangeState.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UBTD_CanChangeState : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	ECharacterStateType CharacterStateType;
};
