#include "IdleState.h"

void IdleState::Enter(UActionComponent* ActionComponent)
{
}

void IdleState::Tick(UActionComponent* ActionComponent, float DeltaTime)
{
}

void IdleState::Exit(UActionComponent* ActionComponent)
{
    if (AController* Controller = OwnerActor->GetInstigatorController())
    {
        Controller->StopMovement();
    }
}

const FString IdleState::GetStateToString()
{
    return "Idle";
}

bool IdleState::CanChangeState(ECharacterStateType CharacterStateType)
{
    if (CharacterStateType != ECharacterStateType::Idle)
    {
        return true;
    }

    return false;
}

bool IdleState::CanMove() const
{
    return true;
}


