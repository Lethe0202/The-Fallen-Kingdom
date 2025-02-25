#pragma once

#include "CoreMinimal.h"

class UActionComponent;
class ACharacter;
class AActor;

UENUM(BlueprintType)
enum class ECharacterStateType : uint8
{
    Idle = 0,
    Attack = 1,
    Pushed = 2,              //피격이상(넉백, 공중으로 띄우기)
    StatusEffect = 3,        //상태이상(수면, 감전)
    Dead = 4,
};

// Base State Class
class QUARTERVIEWGAME_API ICharacterState
{
public:
    virtual ~ICharacterState() = default; 
    
    virtual void Enter(UActionComponent* ActionComponent) PURE_VIRTUAL(ICharacterState::Enter, );
    virtual void Tick(UActionComponent* ActionComponent, float DeltaTime) PURE_VIRTUAL(ICharacterState::Tick, );
    virtual void Exit(UActionComponent* ActionComponent) PURE_VIRTUAL(ICharacterState::Exit, );
    virtual bool CanChangeState(ECharacterStateType CharacterStateType) = 0;
    
    virtual bool CanMove() const = 0;
    virtual const FString GetStateToString() = 0;
    
    void SetOwnerActor(AActor* NewOwnerActor) {OwnerActor = NewOwnerActor; }
    
protected:
    UPROPERTY()
    TWeakObjectPtr<AActor> OwnerActor;
};
