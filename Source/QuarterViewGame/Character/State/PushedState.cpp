#include "PushedState.h"

#include "../Component/ActionComponent.h"
#include "GameFramework/Character.h"
#include "../Animation/BaseAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "IdleState.h"

void PushedState::Enter(UActionComponent* ActionComponent)
{
    Owner = Cast<ACharacter>(ActionComponent->GetOwner());
}

void PushedState::Tick(UActionComponent* ActionComponent, float DeltaTime)
{
    CurrentStateTime += DeltaTime;
    
    if (Owner != nullptr && CurrentStateTime > 0.5f)
    {
        if (UBaseAnimInstance* AnimInstance = Cast<UBaseAnimInstance>(Owner->GetMesh()->GetAnimInstance()))
        {
            if (!AnimInstance->GetIsFalling() && !AnimInstance->GetIsMoving())
            {
                Owner->GetMesh()->GetAnimInstance()->StopAllMontages(0.5f);
                ActionComponent->ChangeToState<IdleState>();
            }
        }
    }
}

void PushedState::Exit(UActionComponent* ActionComponent)
{
    Owner = nullptr;
}

bool PushedState::CanChangeState(ECharacterStateType CharacterStateType)
{
    bool CanChange = false;

    CanChange |= ECharacterStateType::Idle == CharacterStateType;
    CanChange |= ECharacterStateType::Pushed == CharacterStateType;
    CanChange |= ECharacterStateType::StatusEffect == CharacterStateType;
    
    return CanChange;
}

const FString PushedState::GetStateToString()
{
    return "PushedState";
}

bool PushedState::CanMove() const
{
    return false;
}
