// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_DefaultDash.h"

#include "../Character/BaseCharacter.h"
#include "Components/CapsuleComponent.h"

void UAbility_DefaultDash::ActivateAbility()
{
	Super::ActivateAbility();

	ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwnCharacter());

	Character->RotateLerpCharacter(FRotator(), 0.1f);

	Character->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OnAbilityEnded.AddDynamic(this, &UAbility_DefaultDash::OnChangeCollisionChannle);
	
	PlayMontageToIndex(0);
}

void UAbility_DefaultDash::OnChangeCollisionChannle()
{
	GetOwnCharacter()->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}
