// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckDistanceToTarget.h"

#include "../Character/AICharacter.h"
#include "../Character/Component/ActionComponent.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/BehaviorTreeTypes.h"


UBTS_CheckDistanceToTarget::UBTS_CheckDistanceToTarget()
{
    bNotifyBecomeRelevant = true; // 노드가 활성화될 때 호출
    bNotifyCeaseRelevant = true; // 노드가 비활성화될 때 호출
    Interval = 1.0f;
}

void UBTS_CheckDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    // Blackboard를 통해 데이터 업데이트
    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        //AI 캐릭터와 플레이어 간 거리 계산
        AAIController* AIController = OwnerComp.GetAIOwner();
        if (AIController)
        {
            APawn* ControlledPawn = AIController->GetPawn();
            UObject* TargetObject = BlackboardComp->GetValueAsObject(TargetActorSelector.SelectedKeyName);
            
            if (TargetObject != nullptr)
            {
                AActor* PlayerActor = Cast<AActor>(TargetObject);
                if (ControlledPawn && PlayerActor)
                {
                    float TargetDistance = FVector::Dist(ControlledPawn->GetActorLocation(), PlayerActor->GetActorLocation());
                    BlackboardComp->SetValueAsFloat(DistanceFloatSelector.SelectedKeyName, TargetDistance);
                }
            }         
        }
    }
}
