// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_FocusTarget.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UBTT_FocusTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
protected:
	UPROPERTY(EditAnywhere)
	bool bFocus;
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorKeySelector;
};
