// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../State/ICharacterState.h"
#include "../../Ability/AbilityFunctionLibrary.h"
#include "ActionComponent.generated.h"

//캐릭터가 면역을 가지고있는지
UENUM()
enum class EActionImmunityType : uint8
{
	None				UMETA(DisplayName = "None"),				//면역없음
	PushedImmunity		UMETA(DisplayName = "PushedImmunity"),		//피격면역
};

class ICharacterState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool CanChangeState(ECharacterStateType CharacterStateType);
	void ChangeToState(ECharacterStateType CharacterStateType);
	
	bool TakeEffect(const FEffectType& EffectType);
	
	const bool CanMove() const;
	const TUniquePtr<ICharacterState>& GetCurrentState() const { return CurrentState; }

	const EActionImmunityType& GetActionEffectType() const { return ActionEffectType; }
	
	UFUNCTION(Blueprintcallable)
	const FString GetCurrentStateToString() const { return CurrentState->GetStateToString(); }
	
	template<typename NewStateType>
	void ChangeToState();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	TUniquePtr<ICharacterState> CurrentState;

	UPROPERTY(EditAnywhere)
	EActionImmunityType ActionEffectType;
	
	UPROPERTY(EditAnywhere)
	bool bActivate = true;
};