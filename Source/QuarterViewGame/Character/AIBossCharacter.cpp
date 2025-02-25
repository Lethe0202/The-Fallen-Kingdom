// Fill out your copyright notice in the Description page of Project Settings.


#include "AIBossCharacter.h"

#include "PlayerCharacter.h"
#include "../UI/HUD/PlayerHUD.h"
#include "QuarterViewPlayerController.h"

EMapIconType AAIBossCharacter::GetMapIconType_Implementation() const
{
	return EMapIconType::Boss;
}

EMapIconMovableType AAIBossCharacter::GetMapIconMovableType_Implementation() const
{
	return EMapIconMovableType::Dynamic;
}

float AAIBossCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (AQuarterViewPlayerController* PC = Cast<AQuarterViewPlayerController>(EventInstigator))
	{
		if (UPlayerHUD* PlayerHUD = PC->GetPlayerHUD())
		{
			if (!PlayerHUD->IsVisibleBossHPBar(CharacterID))
			{
				PlayerHUD->InitTopBossHpBar(this);
			}
		}
	}
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AAIBossCharacter::DeadCharacter(AActor* DamageCauser)
{
	Super::DeadCharacter(DamageCauser);

	if (AQuarterViewPlayerController* PC = Cast<AQuarterViewPlayerController>(DamageCauser->GetInstigatorController()))
	{
		if (UPlayerHUD* PlayerHUD = PC->GetPlayerHUD())
		{
			if (!PlayerHUD->IsVisibleBossHPBar(CharacterID))
			{
				PlayerHUD->RemoveTopBossHpBar();
			}
		}
	}
}

const int32 AAIBossCharacter::GetBossHealthBarCnt() const
{
	return BossHealthBarCnt;
}
