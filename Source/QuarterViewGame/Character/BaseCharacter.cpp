// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Component/CharacterStatusComponent.h"
#include "Component/ActionComponent.h"
#include "../Ability/AbilityComponent.h"
#include "../Animation/BaseAnimInstance.h"
#include "State/ICharacterState.h"
#include "State/IdleState.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CharacterStatusComponent = CreateDefaultSubobject<UCharacterStatusComponent>(TEXT("CharacterStatusComponent"));
	ActionComponent = CreateDefaultSubobject<UActionComponent>(TEXT("ActionComponent"));
}

const FText ABaseCharacter::GetCharacterDisplayName() const
{
	return FText();
}

const FVector ABaseCharacter::GetMouseCursorWorldLocation() const
{
	return FVector::ZeroVector;
}

// Called when the game starts or when ,spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetMesh()->GetAnimInstance() != nullptr)
	{
		UBaseAnimInstance* BaseAnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());

		if (BaseAnimInstance != nullptr)
		{
			BaseAnimInstance->OnEndActionDelegate.AddDynamic(this, &ABaseCharacter::OnEndAction);
		}
	}
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::PrimaryAttack()
{
	//Empty
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageAmount > 0)
	{
		if (this->GetTeamType() != Cast<ABaseCharacter>(DamageCauser)->GetTeamType())
		{
			CharacterStatusComponent->TakeDamage(DamageAmount, DamageCauser);

			return DamageAmount;
		}
	}
	else
	{
		if (this->GetTeamType() == Cast<ABaseCharacter>(DamageCauser)->GetTeamType())
		{
			CharacterStatusComponent->TakeDamage(DamageAmount, DamageCauser);

			return DamageAmount;
		}
	}

	return 0.0f;
}

void ABaseCharacter::DeadCharacter(AActor* DamageCauser)
{
	ActionComponent->ChangeToState(ECharacterStateType::Dead);
}

void ABaseCharacter::RotateLerpCharacter(FRotator TargetRotator, float LerpTime)
{
	if (TargetRotator == FRotator::ZeroRotator)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		if (!PC)
		{
			return;
		}
		
		RotationTime = LerpTime;
		RotateStartTime = GetWorld()->GetTimeSeconds();
		StartRotation = GetActorRotation();

		FVector2D CursorPosition;
		PC->GetMousePosition(CursorPosition.X, CursorPosition.Y);

		FVector2D PlayerScreenPosition;
		PC->ProjectWorldLocationToScreen(GetActorLocation(), PlayerScreenPosition);

		FVector2D DirectionVector = CursorPosition - PlayerScreenPosition;
		FVector DirectionVector3D = FVector(DirectionVector.X, DirectionVector.Y, 0.f).GetSafeNormal();
		FRotator FinalRotation = DirectionVector3D.Rotation();
	
		TargetRotation = FRotator(0.f, FinalRotation.Yaw, 0.f);
		TargetRotation.Add(0.f, 90.f, 0.f);

		GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &ABaseCharacter::OnRotationTimer, 0.01f, true);
	}
}

UAbilityComponent* ABaseCharacter::GetAbilityComponent()
{
	return nullptr;
}

void ABaseCharacter::OnEndAction()
{
	if (ActionComponent->CanChangeState(ECharacterStateType::Idle))
	{
		ActionComponent->ChangeToState(ECharacterStateType::Idle);
		
		if (GetAbilityComponent() != nullptr)
		{
			GetAbilityComponent()->ClearTempData();
		}
	}
}

void ABaseCharacter::OnRotationTimer()
{
	float ElapsedTime = GetWorld()->GetTimeSeconds() - RotateStartTime;

	float Alpha = FMath::Clamp(ElapsedTime / RotationTime, 0.0f, 1.0f);
	FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
	SetActorRotation(NewRotation);

	if (ElapsedTime >= RotationTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
	}
}