// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AICharacter.h"
#include "QuarterViewGame/Interface/MapIconInterface.h"
#include "AIBossCharacter.generated.h"

class UAbilityComponent;

UCLASS()
class QUARTERVIEWGAME_API AAIBossCharacter : public AAICharacter, public IMapIconInterface
{
	GENERATED_BODY()
	
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void DeadCharacter(AActor* DamageCauser) override;

	const int32 GetBossHealthBarCnt() const;
	
	UFUNCTION(BlueprintNativeEvent)
	EMapIconType GetMapIconType() const;

	UFUNCTION(BlueprintNativeEvent)
	EMapIconMovableType GetMapIconMovableType() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | Boss", meta = (AllowPrivateAccess = "true"))
	int32 BossHealthBarCnt;
};
