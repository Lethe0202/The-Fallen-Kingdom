// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityFunctionLibrary.h"
#include "../Character/BaseCharacter.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/AnimationManager.h"


#include "GameFramework/Character.h"
#include "../Character/Component/ActionComponent.h"

bool UAbilityFunctionLibrary::ApplyStagger(AActor* EffectedActor, AActor* EffectCauser)
{
	if (UActionComponent* ActionComponent = EffectedActor->GetComponentByClass<UActionComponent>())
	{
		FStaggerEffectType StaggerEffectType;

		StaggerEffectType.EffectAngle = CalculateHitDirection(EffectedActor, EffectCauser);
		
		return ActionComponent->TakeEffect(StaggerEffectType);
	}
	
	return false;
}

bool UAbilityFunctionLibrary::ApplyKnockBackEffect(AActor* EffectedActor, AActor* EffectCauser, FKnockBackEffectType& KnockBackEffectType)
{
	if (UActionComponent* ActionComponent = EffectedActor->GetComponentByClass<UActionComponent>())
	{
		return ActionComponent->TakeEffect(KnockBackEffectType);
	}

	return false;
}

float UAbilityFunctionLibrary::CalculateHitDirection(AActor* EffectedActor, AActor* EffectCauser)
{	
	FVector EffectedActorLocation = EffectedActor->GetActorLocation();
	FVector DirectionVector = (EffectCauser->GetActorLocation() - EffectedActorLocation).GetSafeNormal();
	
	const FVector ForwardVector = EffectedActor->GetActorForwardVector();
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionVector);

	//각도
	float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
	
	//좌,우 방향 판별
	FVector CrossProduct = FVector::CrossProduct(ForwardVector, DirectionVector);

	//0보다 작으면 오른쪽에 있으므로 *=1.f
	if (CrossProduct.Z < 0.f)
	{
		Angle *= -1.f;
	}

	return Angle;
}