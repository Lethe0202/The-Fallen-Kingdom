// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilityFunctionLibrary.generated.h"


USTRUCT(BlueprintType)
struct FEffectType 
{
	GENERATED_BODY()

public:
	FEffectType() {};

public:
	virtual bool IsOfType(int32 InID) const { return FEffectType::TypeID == InID; };

	static const int32 TypeID = 0;
};

USTRUCT(BlueprintType)
struct FKnockBackEffectType : public FEffectType
{
	GENERATED_BODY()

public:
	FKnockBackEffectType()
		: KnockbackXYForce(0.f), KnockbackZForce(0.f)
	{};

public:
	virtual bool IsOfType(int32 InID) const { return FKnockBackEffectType::TypeID == InID; };

	UPROPERTY(EditAnywhere)
	float KnockbackXYForce;

	UPROPERTY(EditAnywhere)
	float KnockbackZForce;

	FVector KnockbackVector;

	static const int32 TypeID = 1;
};


USTRUCT(BlueprintType)
struct FStaggerEffectType : public FEffectType
{
	GENERATED_BODY()

public:
	FStaggerEffectType()
		: EffectAngle(0.f)
	{};
	
	virtual bool IsOfType(int32 InID) const { return FStaggerEffectType::TypeID == InID; };
	
	static const int32 TypeID = 2;

	float EffectAngle;
};

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UAbilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	static bool ApplyStagger(AActor* EffectedActor, AActor* EffectCauser);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	static bool ApplyKnockBackEffect(AActor* EffectedActor, AActor* EffectCauser, FKnockBackEffectType& KnockBackEffectType);

	//방향 계산
	static float CalculateHitDirection(AActor* EffectedActor, AActor* EffectCauser);
	
};
