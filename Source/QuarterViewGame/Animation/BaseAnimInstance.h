// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndActionDelegate);

UCLASS()
class QUARTERVIEWGAME_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	const bool GetIsFalling() const { return bIsFalling; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	const bool GetIsMoving() const { return bMoving; }
	
private:
	UFUNCTION()
	void AnimNotify_EndAction();

public:
	FOnEndActionDelegate OnEndActionDelegate;

protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnCharacter;

	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* CharacterMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	FVector CurrentVelocity = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	float GroundSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bMoving = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character State")
	bool bIsFalling;

};
