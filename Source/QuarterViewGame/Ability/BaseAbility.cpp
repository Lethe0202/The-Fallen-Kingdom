// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"

#include "QVCheatManager.h"
#include "../Common/AbilityStructs.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "../Character/BaseCharacter.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/AnimationManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UBaseAbility::InitStaticData(const FAbilityDataTableRow& InAbilityStaticData)
{
	StaticData.AbilityName = InAbilityStaticData.AbilityName;
	StaticData.Description = InAbilityStaticData.Description;
	StaticData.IconTexture = InAbilityStaticData.IconTexture;
	StaticData.BaseDamage = InAbilityStaticData.BaseDamage;
	StaticData.BaseCooldown = InAbilityStaticData.BaseCooldown;
	StaticData.HoldData = InAbilityStaticData.HoldData;
	StaticData.AbilityActivationType = InAbilityStaticData.AbilityActivationType;
	StaticData.HitParticleSystem = InAbilityStaticData.HitParticleSystem;
	
	StaticData.HoldData = InAbilityStaticData.HoldData;
	StaticData.SpotData = InAbilityStaticData.SpotData;
	StaticData.ChainData = InAbilityStaticData.ChainData;

	ResetRumtimeData();
}

void UBaseAbility::ActivateAbility()
{
	if (StaticData.AbilityActivationType == EAbilityActivationType::Chain)
	{
		RuntimeData.ChainRumtimeData.CurrentChainCnt++;
		OnAbilityStarted.Broadcast();

		//�ִ� ChainCnt üũ
		//True : Chain��Ÿ�� ����
		//False : ��ų ��Ÿ�� ����
		if (RuntimeData.ChainRumtimeData.CurrentChainCnt <= StaticData.ChainData.MaxChainCnt)
		{
			OnUpdateAbilityChainCooling.Broadcast(-1, -1);
			StartChainAbilityCoolingTimer();
		}
		else
		{
			StartAbilityCoolingTimer();
		}
	}
	else if (StaticData.AbilityActivationType == EAbilityActivationType::Combo)
	{
		OnAbilityStarted.Broadcast();
	}
	else
	{
		OnAbilityStarted.Broadcast();

		StartAbilityCoolingTimer();
	}
}

void UBaseAbility::AbilityEffect(AActor* EffectTarget)
{
	//Empty
}

void UBaseAbility::AbilitySpawnHitSystem(FVector Location)
{
	if (UParticleSystem* ParticleSystem = StaticData.HitParticleSystem.LoadSynchronous())
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Location);
	}
}

void UBaseAbility::AdditionalAction(int32 ActionIndex)
{
	//Empty
}

void UBaseAbility::ReleasedInput()
{
	//Empty
}

void UBaseAbility::ResetRumtimeData()
{
	RuntimeData.Reset();
	RuntimeData.bIsActuated = false;

	AlreadyDamagedArray.Empty();
}

void UBaseAbility::StartAbilityCoolingTimer()
{
	if (UQVCheatManager::IsAbilityCoolingTimeZero())
	{
		ResetRumtimeData();
		return;
	}
	
	if (StaticData.BaseCooldown > 0)
	{
		RuntimeData.LastActivationTime = GetWorld()->GetTimeSeconds();
		GetWorld()->GetTimerManager().SetTimer(CoolingTimerHandle, this, &UBaseAbility::OnUpdateAbilityCoolingTimer, GetWorld()->GetDeltaSeconds(), true);
	}
}

void UBaseAbility::StartChainAbilityCoolingTimer()
{
	RuntimeData.LastActivationTime = GetWorld()->GetTimeSeconds();

	if (GetWorld()->GetTimerManager().IsTimerActive(ChainCoolingTimerHandle))
	{
		StartAbilityCoolingTimer();
		GetWorld()->GetTimerManager().ClearTimer(ChainCoolingTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ChainCoolingTimerHandle, this, &UBaseAbility::OnUpdateAbilityChainCoolingTimer, GetWorld()->GetDeltaSeconds(), true);
	}
}

/// <summary>
/// /////////����ó�� �ؾ���
/// </summary>
/// <param name="index"></param>
/// <returns></returns>

bool UBaseAbility::PlayMontageToIndex(int index)
{
	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOuter()->GetOuter());
	FName CharacterID = OwnCharacter->GetCharacterID();

	if (UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (OwnCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.IsBound())
		{
			OwnCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		}
		
		if (OwnCharacter->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() != nullptr)
		{
			OwnCharacter->GetMesh()->GetAnimInstance()->StopAllMontages(0.5f);
		}

		if (UAnimationManager* AnimManager = GameInstance->GetAnimManager())
		{
			const FAbilityAnimRow* AbilityAnimRow = AnimManager->GetAbilityMontage(CharacterID, AbilityID);
			UAnimMontage* AbilityMontage = AnimManager->LoadMontage(AbilityAnimRow->AbilityMontages[index]);
			LastPlayMontage = AbilityMontage;
			OwnCharacter->PlayAnimMontage(AbilityMontage);
			
			OwnCharacter->GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &UBaseAbility::OnEndAbility);
			
			return true;
		}
	}

	return false;
}

void UBaseAbility::OnEndAbility(UAnimMontage* Montage, bool bInterrupted)
{
	if (LastPlayMontage == Montage)
	{
		OnAbilityEnded.Broadcast();	
	}
	
	if(StaticData.AbilityActivationType == EAbilityActivationType::Combo)
	{
		StartAbilityCoolingTimer();
	}
}

void UBaseAbility::OnUpdateAbilityCoolingTimer()
{	
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - RuntimeData.LastActivationTime;
	const float Percent = FMath::Clamp(ElapsedTime / StaticData.BaseCooldown, 0.f, 1.f);
	RuntimeData.RemainingCooldown = StaticData.BaseCooldown * (1.f - Percent);
	
	if (RuntimeData.RemainingCooldown <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CoolingTimerHandle);
		ResetRumtimeData();
	}

	OnUpdateAbilityCooling.Broadcast(Percent, RuntimeData.RemainingCooldown);
}

void UBaseAbility::OnUpdateAbilityChainCoolingTimer()
{
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - RuntimeData.LastActivationTime;
	const float Percent = FMath::Clamp(ElapsedTime / StaticData.ChainData.AdditionalInputTime, 0.f, 1.f);
	RuntimeData.ChainRumtimeData.AdditionalInputRemainingTime = StaticData.ChainData.AdditionalInputTime * (1.f - Percent);

	if (RuntimeData.ChainRumtimeData.AdditionalInputRemainingTime <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChainCoolingTimerHandle);

		StartAbilityCoolingTimer();

		RuntimeData.ChainRumtimeData.AdditionalInputRemainingTime = 0.f;
	}

	OnUpdateAbilityChainCooling.Broadcast(Percent, RuntimeData.ChainRumtimeData.AdditionalInputRemainingTime);
}



