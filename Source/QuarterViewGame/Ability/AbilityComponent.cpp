// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "BaseAbility.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/DataTableManager.h"
#include "../Character/BaseCharacter.h"
#include "../Character/Component/ActionComponent.h"
#include "../Character/State/IdleState.h"
#include "../Character/State/AttackState.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...	
}

void UAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInstance != nullptr)
	{
		ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	
		UDataTableManager* DataTableManager = GameInstance->GetDataTableManager();
		for (int32 i = 0; i < StartAbilityClassList.Num(); ++i)
		{
			UBaseAbility* Ability = NewObject<UBaseAbility>(this, StartAbilityClassList[i]);
			AbilityList.Emplace(Ability);
			
			const FAbilityDataTableRow* AbilityStaticData = DataTableManager->GetAbilityStaticData(OwnerCharacter->GetCharacterID(), Ability->GetAbilityID());
			if (AbilityStaticData != nullptr)
			{
				Ability->InitStaticData(*AbilityStaticData);
			}
		}
		
		ActionComponent = GetOwner()->FindComponentByClass<UActionComponent>();
	}
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAbilityComponent::ActivateAbility(const int32 index)
{
	if (!ActionComponent->CanChangeState(ECharacterStateType::Attack))
	{
		return;
	}
	
	CurrentWaitingAbilityIndex = index;
	
	UBaseAbility* BaseAbility = AbilityList[index];
	
	BaseAbility->GetAbilityRuntimeData().bIsActuated = true;
	
	BaseAbility->OnAbilityStarted.Clear();
	BaseAbility->OnAbilityEnded.Clear();
	BaseAbility->OnAbilityStarted.AddDynamic(this, &UAbilityComponent::HandleAbilityStarted);
	BaseAbility->OnAbilityEnded.AddDynamic(this, &UAbilityComponent::HandleAbilityEnded);
	
	BaseAbility->ActivateAbility();
}

void UAbilityComponent::ActivateAbilityFromClass(TSubclassOf<UBaseAbility> AbilityClass)
{
	for (int i = 0; i < AbilityList.Num(); ++i)
	{
		if (AbilityList[i].GetClass() == AbilityClass)
		{
			ActivateAbility(i);
		}
	}
}

void UAbilityComponent::ApplyAbilityEffect(TSubclassOf<UBaseAbility> AbilityClass, const TArray<AActor*>& Targets)
{
	for (int i = 0; i < AbilityList.Num(); ++i)
	{
		if (AbilityList[i].GetClass() == AbilityClass)
		{
			for (int j = 0; j < Targets.Num(); ++j)
			{
				AbilityList[i]->AbilityEffect(Targets[j]);
			}
		}
	}
}

void UAbilityComponent::ApplyAbilityEffect(TSubclassOf<UBaseAbility> AbilityClass, const TArray<AActor*>& Targets, const TArray<FVector>& HitNormal)
{
	for (int i = 0; i < AbilityList.Num(); ++i)
	{
		if (AbilityList[i].GetClass() == AbilityClass)
		{
			for (int j = 0; j < Targets.Num(); ++j)
			{
				AbilityList[i]->AbilityEffect(Targets[j]);
				AbilityList[i]->AbilitySpawnHitSystem(HitNormal[j]);
			}
		}
	}
}

void UAbilityComponent::AbilityAdditionalAction(TSubclassOf<UBaseAbility> AbilityClass, int32 ActionIndex)
{
	for (int i = 0; i < AbilityList.Num(); ++i)
	{
		if (AbilityList[i].GetClass() == AbilityClass)
		{
			AbilityList[i]->AdditionalAction(ActionIndex);			
		}
	}
}

void UAbilityComponent::ClearTempData()
{
	if (CurrentWaitingAbilityIndex != -1)
	{
		UBaseAbility* BaseAbility = AbilityList[CurrentWaitingAbilityIndex];
		BaseAbility->OnAbilityStarted.Clear();
		BaseAbility->OnAbilityEnded.Clear();
	}
	
	bEnableComboInput = false;
	bInputNextCombo = false;
	CurrentWaitingAbilityIndex = -1;
}

void UAbilityComponent::HandleAbilityStarted()
{
	if (ActionComponent)
	{
		ActionComponent->ChangeToState(ECharacterStateType::Attack);	
	}
}

void UAbilityComponent::HandleAbilityEnded()
{
	if (ActionComponent)
	{
		if (ActionComponent->CanChangeState(ECharacterStateType::Idle))
		{
			ActionComponent->ChangeToState(ECharacterStateType::Idle);	
		}
	}
	
	ClearTempData();
}

