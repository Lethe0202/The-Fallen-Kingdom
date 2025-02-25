// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_WhirlWind.h"

#include "../Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAbility_WhirlWind::ActivateAbility()
{
	Super::ActivateAbility();

	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());
	PlayMontageToIndex(0);
	
	MovementDirectionVector = (OwnCharacter->GetMouseCursorWorldLocation() - OwnCharacter->GetActorLocation()).GetSafeNormal();
}

void UAbility_WhirlWind::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
	
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, Cast<APawn>(EffectTarget)->GetController(), GetOwnCharacter(), nullptr);
}

void UAbility_WhirlWind::AdditionalAction(int32 ActionIndex)
{
	if (ActionIndex == 0)
	{
		GetWorld()->GetTimerManager().SetTimer(MovementTimerhandle, this, &UAbility_WhirlWind::OnMovementTimer, GetWorld()->GetDeltaSeconds(), true);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(MovementTimerhandle);
	}
}

void UAbility_WhirlWind::OnMovementTimer()
{
	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());
	OwnCharacter->AddMovementInput(MovementDirectionVector, MovementSpeed);
}
