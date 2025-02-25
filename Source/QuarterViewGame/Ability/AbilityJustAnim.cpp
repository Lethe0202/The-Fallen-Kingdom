// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityJustAnim.h"

#include "../Character/BaseCharacter.h"
#include "AbilityFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UAbilityJustAnim::ActivateAbility()
{
	Super::ActivateAbility();

	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());

	PlayMontageToIndex(0);
}

void UAbilityJustAnim::AbilityEffect(AActor* EffectTarget)
{
	if (bStagger)
	{
		UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
	}
	
	AActor* OwnActor = Cast<AActor>(GetOuter()->GetOuter());
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, Cast<APawn>(EffectTarget)->GetController(), OwnActor, nullptr);
}
