// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_ComboSmash.h"

#include "AbilityFunctionLibrary.h"
#include "../Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAbility_ComboSmash::ActivateAbility()
{
	Super::ActivateAbility();

	Cast<ABaseCharacter>(GetOwnCharacter())->RotateLerpCharacter(FRotator(), 0.25f);

	PlayMontageToIndex(0);
}

void UAbility_ComboSmash::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);

	UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
}

void UAbility_ComboSmash::AdditionalAction(int32 ActionIndex)
{
	Super::AdditionalAction(ActionIndex);
	
	Cast<ABaseCharacter>(GetOwnCharacter())->RotateLerpCharacter(FRotator(), 0.25f);
}
