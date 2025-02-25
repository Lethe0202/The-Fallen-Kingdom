// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_BladeImpact.h"
#include "../Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAbility_BladeImpact::ActivateAbility()
{
	Super::ActivateAbility();

	PlayMontageToIndex(0);

	Cast<ABaseCharacter>(GetOwnCharacter())->RotateLerpCharacter(FRotator(), 0.25f);
}

void UAbility_BladeImpact::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
	
	float DamageResult = StaticData.BaseDamage;
	KnockBackEffectType.KnockbackVector = (EffectTarget->GetActorLocation() - GetOwnCharacter()->GetActorLocation()).GetSafeNormal() * KnockBackEffectType.KnockbackXYForce;
	
	if (RuntimeData.CurrentEffectCount == 2)
	{
		KnockBackEffectType.KnockbackVector.Z = KnockBackEffectType.KnockbackZForce;
		DamageResult *= 1.5f;
	}
	else
	{
		KnockBackEffectType.KnockbackVector.Z = 0.f;
	}
	
	UAbilityFunctionLibrary::ApplyKnockBackEffect(EffectTarget, Cast<AActor>(GetOwnCharacter()), KnockBackEffectType);
	UGameplayStatics::ApplyDamage(EffectTarget, DamageResult, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
}

void UAbility_BladeImpact::AdditionalAction(int32 ActionIndex)
{
	++RuntimeData.CurrentEffectCount;
}
