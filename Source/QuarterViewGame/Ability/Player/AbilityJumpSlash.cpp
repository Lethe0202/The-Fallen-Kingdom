// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityJumpSlash.h"

#include "../Character/BaseCharacter.h"
#include "AbilityFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

void UAbilityJumpSlash::ActivateAbility()
{
	Super::ActivateAbility();

	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());
	
	OwnCharacter->RotateLerpCharacter(FRotator(), 0.2f);
	
    LaunchVectorToMouseCursor();

	PlayMontageToIndex(0);
}

void UAbilityJumpSlash::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);

	FVector DirectionVector = (EffectTarget->GetActorLocation() - GetOwnCharacter()->GetActorLocation()).GetSafeNormal();
	
	AActor* OwnActor = Cast<AActor>(GetOuter()->GetOuter());
	KnockBackEffectType.KnockbackVector = DirectionVector * KnockBackEffectType.KnockbackXYForce;
	KnockBackEffectType.KnockbackVector.Z = KnockBackEffectType.KnockbackZForce;
	
	UAbilityFunctionLibrary::ApplyKnockBackEffect(EffectTarget, OwnActor, KnockBackEffectType);
	UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
}

void UAbilityJumpSlash::LaunchVectorToMouseCursor()
{
    ACharacter* Character = GetOwnCharacter();
    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	
    if (!PlayerController)
    {
        return;
    }
	
    FHitResult HitResult;
	
    if (!PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, HitResult))
    {
        return;
    }
	
    FVector TargetLocation = HitResult.ImpactPoint;
	TargetLocation.Z = 90.f;
    FVector StartLocation = Character->GetActorLocation();
	FVector LaunchVelocity;
	
	if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(Character->GetWorld(), LaunchVelocity, StartLocation, TargetLocation, 0.f, 0.75f))
	{
		Character->LaunchCharacter(LaunchVelocity, true, true);
	}
}
