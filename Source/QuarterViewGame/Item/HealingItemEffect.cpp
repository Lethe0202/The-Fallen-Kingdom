// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItemEffect.h"

#include "Component/CharacterStatusComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UHealingItemEffect::ApplyEffect(ACharacter* TargetCharacter)
{
	UGameplayStatics::ApplyDamage(TargetCharacter, -HealAmount, nullptr, TargetCharacter, nullptr);
	UE_LOG(LogTemp, Warning, TEXT("HealingItemEffect::ApplyEffect"));
}
