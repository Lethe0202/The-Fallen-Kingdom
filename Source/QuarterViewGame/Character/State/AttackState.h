#pragma once

#include "CoreMinimal.h"
#include "ICharacterState.h"

class QUARTERVIEWGAME_API AttackState : public ICharacterState
{
public:
    AttackState() = default;
    virtual ~AttackState() override = default;

    virtual void Enter(UActionComponent* ActionComponent) override;
    virtual void Tick(UActionComponent* ActionComponent, float DeltaTime) override;
    virtual void Exit(UActionComponent* ActionComponent) override;
    virtual bool CanChangeState(ECharacterStateType CharacterStateType) override;
    virtual const FString GetStateToString() override;
    
    virtual bool CanMove() const override;

    static TUniquePtr<ICharacterState> Create() { return MakeUnique<AttackState>(); }
};
