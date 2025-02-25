// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityChargeSmash.h"

#include "AbilityFunctionLibrary.h"
#include "../Character/BaseCharacter.h"

#include "Kismet/GameplayStatics.h"

void UAbilityChargeSmash::ActivateAbility()
{
	Super::ActivateAbility();

	Cast<ABaseCharacter>(GetOwnCharacter())->RotateLerpCharacter(FRotator(), 0.25f);

	PlayMontageToIndex(0);
}

void UAbilityChargeSmash::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);

	UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
	
	if (RuntimeData.HoldingRuntimeData.bPerfectZone)
	{
		UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage * 1.5f, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
	}
	else
	{
		UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
	}
}

void UAbilityChargeSmash::ReleasedInput()
{
	if (UAnimInstance* AnimInstance = Cast<ACharacter>(GetOuter()->GetOuter())->GetMesh()->GetAnimInstance())
	{
		if (UAnimMontage* CurrentMontage = AnimInstance->GetCurrentActiveMontage())
		{
			AnimInstance->Montage_JumpToSection("Smash", CurrentMontage);
		}
	}
}
