// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySlash_1.h"

#include "../Character/BaseCharacter.h"
#include "../Ability/Utility/AbilityEffectHandleActor.h"
#include "AbilityFunctionLibrary.h"
#include "../../Common/QVSystemLibrary.h"

#include "Kismet/GameplayStatics.h"

void UAbilitySlash_1::ActivateAbility()
{
	Super::ActivateAbility();

	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());
	PlayMontageToIndex(0);

	OwnCharacter->RotateLerpCharacter(OwnCharacter->GetActorRotation());
}

void UAbilitySlash_1::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
}

void UAbilitySlash_1::AdditionalAction(int32 ActionIndex)
{
	if (ActionIndex == 0)
	{
		if (DoTActorClass == nullptr)
		{
			return;
		}

		ACharacter* Character = GetOwnCharacter();

		FVector WorldOffset = Character->GetActorRotation().RotateVector(SpawnOffSet);
		FVector SpawnLocation = Character->GetActorLocation() + WorldOffset;

		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SpawnLocation);
		
		AbilityEffectHandleActor = GetWorld()->SpawnActor<AAbilityEffectHandleActor>(DoTActorClass, SpawnTransform);	
		AbilityEffectHandleActor->OnExecuteEffectTarget.AddDynamic(this, &UAbilitySlash_1::OnDoTHandle);
		
		GetWorld()->GetTimerManager().SetTimer(DoTDestroyTimerHandle, this, &UAbilitySlash_1::OnDoTDestroy, ActorLifeTime, false);
	}
}

void UAbilitySlash_1::OnDoTHandle(AActor* Target)
{
	UGameplayStatics::ApplyDamage(Target, StaticData.BaseDamage * 0.5f, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
}

void UAbilitySlash_1::OnDoTDestroy()
{
	AbilityEffectHandleActor->Destroy();
}
