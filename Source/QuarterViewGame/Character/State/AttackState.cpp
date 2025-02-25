#include "AttackState.h"

#include "../Component/ActionComponent.h"

void AttackState::Enter(UActionComponent* ActionComponent)
{
}

void AttackState::Tick(UActionComponent* ActionComponent, float DeltaTime)
{
}

void AttackState::Exit(UActionComponent* ActionComponent)
{
}

bool AttackState::CanChangeState(ECharacterStateType CharacterStateType)
{
    bool CanChange = false;

    if (UActionComponent* ActionComponent = OwnerActor->FindComponentByClass<UActionComponent>())
    {
        if (ActionComponent->GetActionEffectType() == EActionImmunityType::None)
        {
            CanChange |= ECharacterStateType::Idle == CharacterStateType;
            CanChange |= ECharacterStateType::Pushed == CharacterStateType;
            CanChange |= ECharacterStateType::Dead == CharacterStateType;
        }
        else
        {
            CanChange |= ECharacterStateType::Idle == CharacterStateType;
            CanChange |= ECharacterStateType::Dead == CharacterStateType;
        }
    }
    
    return CanChange;
}

const FString AttackState::GetStateToString()
{
    return "AttackState";
}

bool AttackState::CanMove() const
{
    return false;
}
