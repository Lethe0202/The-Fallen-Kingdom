// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "QVAIController.generated.h"

class AAICharacter;
class UBehaviorTreeComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Damage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUnPossessed);

UCLASS()
class QUARTERVIEWGAME_API AQVAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AQVAIController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	UFUNCTION()
	void OnPerceptionUpdated(AActor* InActor, FAIStimulus const Stimulus);

public:
	AActor* GetTargetActor();

	UPROPERTY(BlueprintAssignable)
	FOnUnPossessed OnUnPossessed;
	
protected:
	UPROPERTY()
	AAICharacter* OwnAICharacter;

	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AISetting, meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AISetting, meta = (AllowPrivateAccess = "true"))
	UAISenseConfig_Damage* DamageConfig;
};
