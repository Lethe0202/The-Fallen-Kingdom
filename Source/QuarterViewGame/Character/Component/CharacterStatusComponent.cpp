// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatusComponent.h"
#include "../BaseCharacter.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/AnimationManager.h"
#include "../Manager/DataTableManager.h"
#include "../Manager/SaveManager.h"
#include "../../Common/QuarterViewGameDataTable.h"

#include "Perception/AISenseConfig_Damage.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UCharacterStatusComponent::UCharacterStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}

void UCharacterStatusComponent::InitializeComponent()
{
	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOwner());
	FName CharacterID = OwnCharacter->GetCharacterID();

	if (UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		UDataTableManager* DataTableManager = GameInstance->GetDataTableManager();
		
		const FCharacterStatusaRow* CharacterStatusaRow = DataTableManager->GetCharacterPrimaryStatus(CharacterID);
		
		if (CharacterStatusaRow != nullptr)
		{
			MaxHP = CharacterStatusaRow->MaxHp;
			CurrentHP = MaxHP;
		}
	}
}

// Called when the game starts
void UCharacterStatusComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStatusComponent::TakeDamage(float DamageValue, AActor* DamageCauser)
{
	if (!bActivate)
	{
		return;
	}
	
	if (CurrentHP <= 0)
	{
		return;
	}

	ABaseCharacter* OwnCharacter = Cast<ABaseCharacter>(GetOwner());
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());

	if (DamageValue > 0)
	{
		UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), DamageCauser, DamageValue, DamageCauser->GetActorLocation(), DamageCauser->GetActorLocation());
	}
	
	CurrentHP -= DamageValue;

	if (CurrentHP > MaxHP)
	{
		CurrentHP = MaxHP;
	}
	
	float HealthPercent = CurrentHP / MaxHP;

	OnHealthChanged.Broadcast(HealthPercent);

	if (CurrentHP <= 0)
	{
		UAnimMontage* DeadMontage = GameInstance->GetAnimManager()->GetActionAnimMontage(OwnCharacter->GetCharacterID(), EActionAnimationType::Dead);
		
		ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
		OwnerCharacter->PlayAnimMontage(DeadMontage);
		
		OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName(FName("Dead"));
		OwnCharacter->DeadCharacter(DamageCauser);
	}
}

float UCharacterStatusComponent::CalculateHitDirection(FVector TargetLocation)
{	
	//��ġ
	FVector OwnLocation = GetOwner()->GetActorLocation();

	//����
	FVector DirectionVector = (TargetLocation - OwnLocation).GetSafeNormal();

	const FVector ForwardVector = GetOwner()->GetActorForwardVector();

	//���� ���
	float DotProduct = FVector::DotProduct(ForwardVector, DirectionVector);
	float Angle = FMath::RadiansToDegrees(FMath::Acos(DotProduct));

	FVector CrossProduct = FVector::CrossProduct(ForwardVector, DirectionVector);

	if (CrossProduct.Z < 0.f)
	{
		Angle *= -1.f;
	}

	return Angle;
}

void UCharacterStatusComponent::LoadData(USaveGameDataBase& LoadData)
{
	CurrentHP = Cast<UCharacterStatusSaveData>(&LoadData)->CurrentHealth;
	OnHealthChanged.Broadcast(CurrentHP / MaxHP);
}

void UCharacterStatusComponent::SaveData(USaveGameDataBase*& SaveData)
{
	SaveData = NewObject<UCharacterStatusSaveData>();
	Cast<UCharacterStatusSaveData>(SaveData)->CurrentHealth = CurrentHP;
}

ESaveDataType UCharacterStatusComponent::SaveDataType()
{
	return ESaveDataType::CharacterStatus;
}

