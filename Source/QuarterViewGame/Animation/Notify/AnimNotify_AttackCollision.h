// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_AttackCollision.generated.h"

UENUM(BlueprintType)
enum class EShape : uint8
{
	Box				UMETA(DisplayName = "Box"),
	Sphere			UMETA(DisplayName = "Sphere"),
	Capsule			UMETA(DisplayName = "Capsule"),
};

USTRUCT(BlueprintType)
struct FAttackCollisionShape
{
	GENERATED_USTRUCT_BODY()

	FAttackCollisionShape()
	{
		ShapeType = EShape::Box;
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Box"))
	float HalfExtentX = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Box"))
	float HalfExtentY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Box"))
	float HalfExtentZ = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Sphere"))
	float SphereRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Capsule"))
	float CapsuleRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape", meta = (EditCondition = "ShapeType == EShape::Capsule"))
	float HalfHeight = 0.f;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionShape")
	EShape ShapeType;
};

UCLASS()
class QUARTERVIEWGAME_API UAnimNotify_AttackCollision : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
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
};
