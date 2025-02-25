// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_WindBlade.h"

#include "AbilityFunctionLibrary.h"
#include "../Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAbility_WindBlade::ActivateAbility()
{
	Super::ActivateAbility();

	PlayMontageToIndex(0);

	Cast<ABaseCharacter>(GetOwnCharacter())->RotateLerpCharacter(FRotator(), 0.25f);
}

void UAbility_WindBlade::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
	
	KnockBackEffectType.KnockbackVector = GetOwnCharacter()->GetActorForwardVector() * KnockBackEffectType.KnockbackXYForce;
	KnockBackEffectType.KnockbackVector.Z = KnockBackEffectType.KnockbackZForce;
	
	UAbilityFunctionLibrary::ApplyKnockBackEffect(EffectTarget, Cast<AActor>(GetOwnCharacter()), KnockBackEffectType);
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
}


