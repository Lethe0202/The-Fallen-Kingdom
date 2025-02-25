// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacter.h"

#include "ItemManager.h"
#include "PlayerCharacter.h"
#include "Component/ActionComponent.h"
#include "../Ability/AbilityComponent.h"
#include "../UI/HUD/HUDHealthBarWidget.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/AnimationManager.h"
#include "../Controller/QVAIController.h"

#include "Math/UnrealMathUtility.h"
#include "Components/WidgetComponent.h"


AAICharacter::AAICharacter()
{
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComponent"));

	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(GetMesh());
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	if (UHUDHealthBarWidget* HUDHealthBarWidget = Cast<UHUDHealthBarWidget>(HealthWidgetComponent->GetWidget()))
	{
		HUDHealthBarWidget->Init(CharacterStatusComponent);
	}
}

void AAICharacter::PrimaryAttack()
{
	Super::PrimaryAttack();

	if (!ActionComponent->CanChangeState(ECharacterStateType::Attack))
	{
		return;
	}
	
	UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	FString AbilityString = "PrimaryAttack";
	
	const FAbilityAnimRow* AbilityAnimRow = QuarterViewGameInstance->GetAnimManager()->GetAbilityMontage(CharacterID, *AbilityString);
	if (AbilityAnimRow != nullptr)
	{
		int32 AnimCnt = AbilityAnimRow->AbilityMontages.Num();
		int32 RandIndex = FMath::RandRange(0, AnimCnt - 1);
		
		UAnimMontage* AttackMontage = AbilityAnimRow->AbilityMontages[RandIndex].LoadSynchronous();
		
		ActionComponent->ChangeToState(ECharacterStateType::Attack);

		GetMesh()->GetAnimInstance()->OnMontageEnded.Clear();
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
	}
}

void AAICharacter::DeadCharacter(AActor* DamageCauser)
{
	Super::DeadCharacter(DamageCauser);

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(DamageCauser))
	{
		PlayerCharacter->OnWorkObjectiveID.Broadcast(CharacterID);
	}
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->UnPossess();
	}
	
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetGameInstance()))
	{
		QuarterViewGameInstance->GetItemManager()->DropItemToCharacterID(CharacterID, GetActorLocation());
	}
}

UAbilityComponent* AAICharacter::GetAbilityComponent()
{
	return AbilityComponent ? AbilityComponent : nullptr;
}

const FText AAICharacter::GetCharacterDisplayName() const
{
	return CharacterDisplayName;
}

const FVector AAICharacter::GetMouseCursorWorldLocation() const
{
	if (Cast<AQVAIController>(Controller) != nullptr)
	{
		AActor* TargetActor = Cast<AQVAIController>(Controller)->GetTargetActor();

		if (TargetActor != nullptr)
		{
			return TargetActor->GetActorLocation();
		}
		else
		{
			return FVector::ZeroVector;
		}
	}

	return FVector::ZeroVector;
}
