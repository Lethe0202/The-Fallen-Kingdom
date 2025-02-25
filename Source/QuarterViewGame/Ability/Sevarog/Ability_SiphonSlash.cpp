// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SiphonSlash.h"
#include "../Character/BaseCharacter.h"

#include "Kismet/GameplayStatics.h"
#include "AbilityFunctionLibrary.h"

void UAbility_SiphonSlash::ActivateAbility()
{
	Super::ActivateAbility();

	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());
	PlayMontageToIndex(0);
}

void UAbility_SiphonSlash::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
	
	UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, Cast<APawn>(EffectTarget)->GetController(), GetOwnCharacter(), nullptr);
}
