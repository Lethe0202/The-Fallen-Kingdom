// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	
	if (IsValid(TryGetPawnOwner()))
	{
		OwnCharacter = Cast<ACharacter>(TryGetPawnOwner());
		CharacterMovementComponent = OwnCharacter->GetCharacterMovement();
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (OwnCharacter != nullptr)
	{
		CurrentVelocity = CharacterMovementComponent->Velocity;
		GroundSpeed = FMath::Sqrt(CurrentVelocity.X * CurrentVelocity.X + CurrentVelocity.Y * CurrentVelocity.Y);
		
		bool bAcceleration = !CharacterMovementComponent->GetCurrentAcceleration().Equals(FVector(0, 0, 0), 0.f);
		
		bMoving = bAcceleration || (GroundSpeed > 3.f);
		bIsFalling = CharacterMovementComponent->IsFalling();
	}
}

void UBaseAnimInstance::AnimNotify_EndAction()
{
	OnEndActionDelegate.Broadcast();
}
