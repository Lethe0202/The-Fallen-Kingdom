// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_GhostHand.h"

#include "AbilityFunctionLibrary.h"
#include "Utility/AbilityEffectHandleActor.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Character.h"
#include "NavigationSystem.h"

void UAbility_GhostHand::ActivateAbility()
{
	Super::ActivateAbility();

    ACharacter* OwnerCharacter = GetOwnCharacter();

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation system not found."));
        return;
    }
    
    FVector Origin = OwnerCharacter->GetActorLocation();
    float Radius = 500.f;
    FNavLocation RandomNavLocation;
    
    for (int i = 0; i < SpawnCount; ++i)
    {
        bool bFound = NavSystem->GetRandomReachablePointInRadius(Origin, Radius, RandomNavLocation);
        
        float RandomYaw = FMath::FRandRange(0.f, 360.f);
        FRotator RandomRotation(0.f, RandomYaw, 0.f);
        
        if (bFound)
        {
            FTransform SpawnTransform;
            SpawnTransform.SetLocation(RandomNavLocation.Location);
            SpawnTransform.SetRotation(RandomRotation.Quaternion());
            
            AAbilityEffectHandleActor* AbilityEffectHandleActor = GetWorld()->SpawnActor<AAbilityEffectHandleActor>(GhostHandActorClass, SpawnTransform);
            AbilityEffectHandleActor->OnExecuteEffectTarget.AddDynamic(this, &UAbility_GhostHand::OnGhostHandEffect);
            
            AbilityEffectHandleActor->SetDamage(StaticData.BaseDamage);
        }
    }
    
    OnAbilityEnded.Broadcast();
}

void UAbility_GhostHand::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);
    
    UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
    UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, Cast<APawn>(EffectTarget)->GetController(), GetOwnCharacter(), nullptr);
}

void UAbility_GhostHand::OnGhostHandEffect(AActor* TargetActor)
{
    AbilityEffect(TargetActor);
    AbilitySpawnHitSystem(TargetActor->GetActorLocation());
}
