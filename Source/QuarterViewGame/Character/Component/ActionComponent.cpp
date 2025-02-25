// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionComponent.h"
#include "../Character/BaseCharacter.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/AnimationManager.h"
#include "../../Ability/AbilityFunctionLibrary.h"

#include "State/ICharacterState.h"
#include "State/IdleState.h"
#include "State/AttackState.h"
#include "State/PushedState.h"
#include "State/DeadState.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UActionComponent::UActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UActionComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentState = MakeUnique<IdleState>();
	CurrentState->SetOwnerActor(GetOwner());
}

// Called every frame
void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CurrentState)
	{
		CurrentState->Tick(this, DeltaTime);
	}
}

bool UActionComponent::CanChangeState(ECharacterStateType CharacterStateType)
{
	return CurrentState->CanChangeState(CharacterStateType);
}

void UActionComponent::ChangeToState(ECharacterStateType CharacterStateType)
{
	switch (CharacterStateType)
	{
	case ECharacterStateType::Idle:
		ChangeToState<IdleState>();
		break;

	case ECharacterStateType::Attack:
		ChangeToState<AttackState>();
		break;

	case ECharacterStateType::Pushed:
		ChangeToState<PushedState>();
		break;

	case ECharacterStateType::Dead:
		ChangeToState<DeadState>();
		break;
	}
}

template<typename NewStateType>
void UActionComponent::ChangeToState()
{
	if (CurrentState)
	{
		CurrentState->Exit(this);
	}
		
	CurrentState = NewStateType::Create();
	CurrentState->Enter(this);
	CurrentState->SetOwnerActor(GetOwner());
}

bool UActionComponent::TakeEffect(const FEffectType& EffectType)
{
	if (!bActivate)
	{
		return false;
	}
	
	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOwner());
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());

	if (EffectType.IsOfType(FKnockBackEffectType::TypeID))
	{	
		if (CanChangeState(ECharacterStateType::Pushed))
		{
			FKnockBackEffectType* const KnockBackEffectType = (FKnockBackEffectType*)&EffectType;

			if (KnockBackEffectType->KnockbackVector.Z >= 100)
			{
				UAnimMontage* KnockbackMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::Knockback);
				OwnCharacter->GetMesh()->GetAnimInstance()->Montage_Play(KnockbackMontage);
			}
			else
			{
				UAnimMontage* HitMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::HitFront);
				OwnCharacter->GetMesh()->GetAnimInstance()->Montage_Play(HitMontage);
			}
			
			OwnCharacter->GetCharacterMovement()->Velocity.Set(0.f, 0.f, 0.f);
			OwnCharacter->GetCharacterMovement()->StopActiveMovement();
			
			Cast<ACharacter>(GetOwner())->LaunchCharacter(KnockBackEffectType->KnockbackVector, true, true);
			
			ChangeToState(ECharacterStateType::Pushed);
			
			return true;
		}
	}
	else if (EffectType.IsOfType(FStaggerEffectType::TypeID))
	{
		if (!CanChangeState(ECharacterStateType::Pushed))
		{
			return false;
		}
		
		FStaggerEffectType* const StaggerEffectType = (FStaggerEffectType*)&EffectType;
		float Angle = StaggerEffectType->EffectAngle;
	
		UAnimMontage* HitMontage;
		
		//앞
		if (-60.f < Angle && Angle < 60.f)
		{
			HitMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::HitFront);
		}
		//오른쪽
		else if(60.f < Angle && Angle < 120.f)
		{
			HitMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::HitRight);
		}
		//뒤
		else if (-120.f < Angle && Angle < -60.f)
		{
			HitMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::HitBack);
		}
		//왼쪽
		else
		{
			HitMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::HitLeft);
		}
	
		OwnCharacter->PlayAnimMontage(HitMontage);
		ChangeToState(ECharacterStateType::Pushed);
		
		return true;
	}

	return false;
}

const bool UActionComponent::CanMove() const
{
	return CurrentState->CanMove();
}

