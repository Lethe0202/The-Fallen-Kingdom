// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_RandomNumToPercent.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UBTS_RandomNumToPercent : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

protected:
	//각 인덱스의 확률을 0 ~ 1 사이로 채워넣는다. 
	UPROPERTY(EditAnywhere)
	TArray<float> Percents;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector RandomNumKeySelector;
};
