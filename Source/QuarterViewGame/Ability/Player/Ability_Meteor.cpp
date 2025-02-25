// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_Meteor.h"

#include "AbilityFunctionLibrary.h"
#include "Utility/AbilityEffectHandleActor.h"
#include "../../Common/QVSystemLibrary.h"
#include "GameFramework/Character.h"

#include "Kismet/GameplayStatics.h"

void UAbility_Meteor::ActivateAbility()
{
	Super::ActivateAbility();

	if (MeteorFXActorClass != nullptr)
	{
		FVector SpawnLocation = RuntimeData.SpotRumtimeData.SpotLocation;
		FRotator Rotation = FRotator::ZeroRotator;
		
		AAbilityEffectHandleActor* EffectHandleActor = GetWorld()->SpawnActor<AAbilityEffectHandleActor>(MeteorFXActorClass, SpawnLocation, Rotation);
		
		EffectHandleActor->OnExecuteEffectLocation.AddDynamic(this, &UAbility_Meteor::OnMeteorEffect);
	}
	
	OnAbilityEnded.Broadcast();
}

void UAbility_Meteor::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);

	UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);	
}

void UAbility_Meteor::OnMeteorEffect(const FVector& Location)
{
	FCollisionShape Shape = FCollisionShape::MakeSphere(StaticData.SpotData.Radius);
	
	TArray<FHitResult> Results;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Cast<AActor>(GetOwnCharacter()));

	Shape = FCollisionShape::MakeSphere(StaticData.SpotData.Radius);
	UQVSystemLibrary::DrawDebugSphere(GetWorld(), Location, StaticData.SpotData.Radius, 12, FColor::Red, 3.f);
	
	bool bHit = GetWorld()->SweepMultiByChannel(Results, Location, Location, FQuat::Identity, ECC_Pawn, Shape, Params);
	if (bHit)
	{
		int HitResultCnt = Results.Num();
		for (int i = 0; i < HitResultCnt; ++i)
		{
			if (AActor* HitActor = Results[i].GetActor())
			{
				AbilityEffect(HitActor);
				AbilitySpawnHitSystem(HitActor->GetActorLocation());
			}
		}
	}
}
