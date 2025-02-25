// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckDistanceToTarget.generated.h"

/**
 * 
 */
//return true : Distance > TargetDistance, false : Distance < TargetDistance
UCLASS()
class QUARTERVIEWGAME_API UBTS_CheckDistanceToTarget : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTS_CheckDistanceToTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector TargetActorSelector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector DistanceFloatSelector;
};
