// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TryActivateAbility.generated.h"

class UBaseAbility;

UCLASS()
class QUARTERVIEWGAME_API UBTT_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

private:
	UPROPERTY(EditAnywhere, meta =(AllowPrivateAccess = "true"))
	TSubclassOf<UBaseAbility> AbilityClass;
};
