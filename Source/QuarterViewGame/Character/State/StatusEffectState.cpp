#include "StatusEffectState.h"

#include "../Component/ActionComponent.h"

void StatusEffectState::Enter(UActionComponent* ActionComponent)
{
}

void StatusEffectState::Tick(UActionComponent* ActionComponent, float DeltaTime)
{
}

void StatusEffectState::Exit(UActionComponent* ActionComponent)
{
}

bool StatusEffectState::CanChangeState(ECharacterStateType CharacterStateType)
{
    bool CanChange = false;

    return CanChange |= ECharacterStateType::Idle == CharacterStateType;
}

const FString StatusEffectState::GetStateToString()
{
    return "StatusEffectState";
}

bool StatusEffectState::CanMove() const
{
    return false;
}
