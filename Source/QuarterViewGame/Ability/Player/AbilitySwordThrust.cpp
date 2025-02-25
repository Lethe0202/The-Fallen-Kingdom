// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySwordThrust.h"

#include "AbilityFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "Components/CapsuleComponent.h"

void UAbilitySwordThrust::ActivateAbility()
{
	Super::ActivateAbility();

	if (RuntimeData.ChainRumtimeData.CurrentChainCnt == 1)
	{
		ACharacter* Character = GetOwnCharacter();

		RotateStartTime = GetWorld()->GetTimeSeconds();
		StartRotation = Character->GetActorRotation();

		FVector2D CursorPosition;
		Cast<APlayerController>(Character->GetController())->GetMousePosition(CursorPosition.X, CursorPosition.Y);

		FVector2D PlayerScreenPosition;
		Cast<APlayerController>(Character->GetController())->ProjectWorldLocationToScreen(Character->GetActorLocation(), PlayerScreenPosition);

		FVector2D DirectionVector = CursorPosition - PlayerScreenPosition;

		FVector DirectionVector3D = FVector(DirectionVector.X, DirectionVector.Y, 0.f).GetSafeNormal();

		// ���� ���� ���� ���Ϳ��� Yaw �� ����
		FRotator FullRotation = DirectionVector3D.Rotation();
		
		// ĳ������ Forward Vector�� ��ǥ ���� �� (Yaw�� ����)
		TargetRotation = FRotator(0.f, FullRotation.Yaw, 0.f);

		TargetRotation.Add(0.f, 90.f, 0.f);
		
		GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &UAbilitySwordThrust::OnRotationTimer, 0.01f, true);
	}
	else
	{
		AlreadyDamagedArray.Empty();

		ACharacter* Character = GetOwnCharacter();

		RotateStartTime = GetWorld()->GetTimeSeconds();
		StartRotation = Character->GetActorRotation();

		FVector DirectionVector = DashStartLocation - Character->GetActorLocation();
		FVector DirectionVector3D = FVector(DirectionVector.X, DirectionVector.Y, 0.f).GetSafeNormal();

		// ���� ���� ���� ���Ϳ��� Yaw �� ����
		FRotator FullRotation = DirectionVector3D.Rotation();
		
		// ĳ������ Forward Vector�� ��ǥ ���� �� (Yaw�� ����)
		TargetRotation = FRotator(0.f, FullRotation.Yaw, 0.f);
		//TargetRotation.Add(0.f, 90.f, 0.f);

		GetWorld()->GetTimerManager().SetTimer(RotationTimerHandle, this, &UAbilitySwordThrust::OnRotationTimer, 0.01f, true);		
	}
}

void UAbilitySwordThrust::AbilityEffect(AActor* EffectTarget)
{
	Super::AbilityEffect(EffectTarget);

	int32 index = AlreadyDamagedArray.Find(EffectTarget);
	
	if (index == -1)
	{
		AlreadyDamagedArray.AddUnique(EffectTarget);

		UAbilityFunctionLibrary::ApplyStagger(EffectTarget, Cast<AActor>(GetOwnCharacter()));
		UGameplayStatics::ApplyDamage(EffectTarget, StaticData.BaseDamage, GetOwnCharacter()->GetController(), GetOwnCharacter(), nullptr);
	}
}

void UAbilitySwordThrust::AdditionalAction(int32 ActionIndex)
{		
	if (RuntimeData.ChainRumtimeData.CurrentChainCnt == 1)
	{
		ACharacter* Character = GetOwnCharacter();

		DashStartLocation = Character->GetActorLocation();
		FVector ForwardVector = Character->GetActorForwardVector();

		DashTargetLocation = DashStartLocation + (ForwardVector * DashDistance);
		DashStartTime = GetWorld()->GetTimeSeconds();

		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAbilitySwordThrust::OnDashTimer, 0.01f, true);

		if (StartLocationFX)
		{
			StartLocationFXComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), StartLocationFX, DashStartLocation);
		}
	}
	else
	{
		ACharacter* Character = GetOwnCharacter();

		//ó�� ��ų�� ����� ��ġ
		DashTargetLocation = DashStartLocation;

		//���� ��ġ
		DashStartLocation = Character->GetActorLocation();
		FVector ForwardVector = Character->GetActorForwardVector();
		
		DashStartTime = GetWorld()->GetTimeSeconds();
		
		GetWorld()->GetTimerManager().SetTimer(DashTimerHandle, this, &UAbilitySwordThrust::OnDashTimer, 0.01f, true);
	}
}

void UAbilitySwordThrust::StartAbilityCoolingTimer()
{
	Super::StartAbilityCoolingTimer();

	if (StartLocationFXComponent)
	{
		StartLocationFXComponent->DestroyComponent();	
	}
}

void UAbilitySwordThrust::ResetRumtimeData()
{
	Super::ResetRumtimeData();

	TargetRotation = FRotator::ZeroRotator;
	StartRotation = FRotator::ZeroRotator;
	RotateStartTime = 0.f;

	DashStartLocation = FVector::ZeroVector;
	DashTargetLocation = FVector::ZeroVector;
	DashStartTime = 0.f;
}

void UAbilitySwordThrust::OnRotationTimer()
{
	ACharacter* Character = GetOwnCharacter();

	float ElapsedTime = GetWorld()->GetTimeSeconds() - RotateStartTime;

	float Alpha = FMath::Clamp(ElapsedTime / RotationTime, 0.0f, 1.0f);
	FRotator NewRotation = FMath::Lerp(StartRotation, TargetRotation, Alpha);
	Character->SetActorRotation(NewRotation);

	if (ElapsedTime >= RotationTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(RotationTimerHandle);
		
		GetOwnCharacter()->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		OnAbilityEnded.AddDynamic(this, &UAbilitySwordThrust::OnChangeCollisionChannle);
		
		PlayMontageToIndex(0);
	}
}

void UAbilitySwordThrust::OnDashTimer()
{
	ACharacter* Character = GetOwnCharacter();

	float ElapsedTime = GetWorld()->GetTimeSeconds() - DashStartTime;
	
	float Alpha = FMath::Clamp(ElapsedTime / DashTime, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(DashStartLocation, DashTargetLocation, Alpha);
	Character->SetActorLocation(NewLocation, true, nullptr, ETeleportType::None);
	
	if (ElapsedTime >= DashTime)
	{
		GetWorld()->GetTimerManager().ClearTimer(DashTimerHandle);
	}
}

void UAbilitySwordThrust::OnChangeCollisionChannle()
{
	GetOwnCharacter()->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}
