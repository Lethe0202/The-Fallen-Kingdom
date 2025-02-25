// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_AttackCollision.h"
#include "AnimNotifyState_AttackCollision.generated.h"

UCLASS()
class QUARTERVIEWGAME_API UAnimNotifyState_AttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference);

protected:
	TArray<AActor*> CheckCollision(USkeletalMeshComponent* MeshComp, TArray<FVector>& OutHitLocations);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Collision")
	FVector LocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Collision")
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Collision")
	FAttackCollisionShape AttackCollisionShape;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Collision")
	TSubclassOf<class UBaseAbility> AbilityClass;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anim | Collision")
	//TObjectPtr<UAnimSequence> AnimSequence;
};
