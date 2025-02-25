// Fill out your copyright notice in the Description page of Project Settings.


#include "QVAIController.h"
#include "../Character/AICharacter.h"

#include "Navigation/CrowdFollowingComponent.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionTypes.h"

AQVAIController::AQVAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("CrowdFollowingComponent")))
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	
	SetPerceptionComponent(*CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception")));
	
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AQVAIController::OnPerceptionUpdated);
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	
	SightConfig->SightRadius = 400.f;
	SightConfig->LoseSightRadius = 1000.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(1.f);

	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
	PerceptionComponent->ConfigureSense(*SightConfig);

	DamageConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	PerceptionComponent->ConfigureSense(*DamageConfig);
}

void AQVAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnAICharacter = Cast<AAICharacter>(InPawn);

	if (IsValid(OwnAICharacter))
	{
		if (IsValid(OwnAICharacter->GetBT()))
		{
			BlackboardComp->InitializeBlackboard(*(OwnAICharacter->GetBT())->BlackboardAsset);
			BehaviorComp->StartTree(*(OwnAICharacter->GetBT()));
		}
	}
}

void AQVAIController::OnUnPossess()
{
	Super::OnUnPossess();

	OnUnPossessed.Broadcast();
}

void AQVAIController::OnPerceptionUpdated(AActor* InActor, FAIStimulus const Stimulus)
{
	if (!Cast<ABaseCharacter>(InActor) || !Cast<ABaseCharacter>(OwnAICharacter))
	{
		return;
	}
	
	auto InSenceID = Stimulus.Type;
	
	if (Cast<ABaseCharacter>(InActor)->GetTeamType() != Cast<ABaseCharacter>(OwnAICharacter)->GetTeamType())
	{
		BlackboardComp->SetValueAsObject("TargetActor", InActor);
	}
	
	if (InSenceID == DamageConfig->GetSenseID())
	{
		if (Stimulus.IsActive() && (Cast<ABaseCharacter>(InActor)->GetTeamType() != Cast<ABaseCharacter>(OwnAICharacter)->GetTeamType()))
		{
			BlackboardComp->SetValueAsObject("TargetActor", InActor);
		}
	}
}

AActor* AQVAIController::GetTargetActor()
{
	UObject* TargetActor = BlackboardComp->GetValueAsObject("TargetActor");
	
	if (TargetActor != nullptr)
	{
		return Cast<AActor>(TargetActor);
	}
	
	return nullptr;
}
