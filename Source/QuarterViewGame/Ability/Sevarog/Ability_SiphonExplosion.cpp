// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_SiphonExplosion.h"

#include "AbilityFunctionLibrary.h"
#include "../Character/BaseCharacter.h"
#include "Kismet/GameplayStatics.h"

void UAbility_SiphonExplosion::ActivateAbility()
{
	Super::ActivateAbility();

	SpawnedCount = 0;
	
	GetWorld()->GetTimerManager().SetTimer(SpawnExplosionTimerHandle, this, &UAbility_SiphonExplosion::OnSpawnExplosionTimer, SpawnDelay, true, 0.f);

	OnAbilityEnded.Broadcast();
}

void UAbility_SiphonExplosion::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
	
	UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, Cast<APawn>(EffectTarget)->GetController(), GetOwnCharacter(), nullptr);
}

void UAbility_SiphonExplosion::OnSpawnExplosionTimer()
{
	++SpawnedCount;
	
	ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwnCharacter());

	FVector SpawnLocation = OwnerCharacter->GetMouseCursorWorldLocation();
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);

	AAbilityEffectHandleActor* AbilityEffectHandleActor = GetWorld()->SpawnActor<AAbilityEffectHandleActor>(SiphonExplosionActorClass, SpawnTransform);
	AbilityEffectHandleActor->OnExecuteEffectTarget.AddDynamic(this, &UAbility_SiphonExplosion::OnSiphonEffect);
	
	if (SpawnedCount >= SpawnCount)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnExplosionTimerHandle);
	}
}

void UAbility_SiphonExplosion::OnSiphonEffect(AActor* TargetActor)
{
	AbilityEffect(TargetActor);
	AbilitySpawnHitSystem(TargetActor->GetActorLocation());
}