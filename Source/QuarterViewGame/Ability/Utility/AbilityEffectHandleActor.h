// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityEffectHandleActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteEffectLoationSignature, const FVector&, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnExecuteEffectTargetSignature, AActor*, Target);

/*
 * 스킬을 통해 스폰하는 Actor
 * 무언가 상호작용(데미지) 시 delegate 브로드캐스트
 */

UCLASS()
class QUARTERVIEWGAME_API AAbilityEffectHandleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityEffectHandleActor();
	
	UFUNCTION()
	void SetDamage(float InDamage) { Damage = InDamage; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnExecuteEffectLoationSignature OnExecuteEffectLocation;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnExecuteEffectTargetSignature OnExecuteEffectTarget;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Damage;
};
