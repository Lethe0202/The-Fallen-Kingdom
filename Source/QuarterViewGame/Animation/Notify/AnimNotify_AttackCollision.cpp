// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AttackCollision.h"

#include "../../Character/BaseCharacter.h"
#include "../../Common/QVSystemLibrary.h"
#include "../Ability/AbilityComponent.h"
#include "../Ability/BaseAbility.h"

void UAnimNotify_AttackCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp)
	{
		return;
	}
	
	TArray<FVector> HitLocation;
	auto TargetActors = CheckCollision(MeshComp, HitLocation);
	
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());
	
	if (BaseCharacter != nullptr)
	{
		if (UAbilityComponent* AbilityComponent = BaseCharacter->GetComponentByClass<UAbilityComponent>())
		{
			AbilityComponent->ApplyAbilityEffect(AbilityClass, TargetActors, HitLocation);
		}
	}
}

TArray<AActor*> UAnimNotify_AttackCollision::CheckCollision(USkeletalMeshComponent* MeshComp, TArray<FVector>& OutHitLocations)
{
	FTransform ShapeTransform;
	
	//소켓 설정
	if (SocketName.IsValid())
	{
		ShapeTransform = MeshComp->GetSocketTransform(SocketName);
		ShapeTransform.SetRotation(MeshComp->GetOwner()->GetActorRotation().Quaternion());
	}
	else
	{
		ShapeTransform = MeshComp->GetOwner()->GetTransform();
		ShapeTransform.SetRotation(MeshComp->GetOwner()->GetActorRotation().Quaternion());
	}
	
	const FQuat CurRelRotQuat = MeshComp->GetOwner()->GetTransform().Rotator().Quaternion();
	const FVector LocalOffset = CurRelRotQuat.RotateVector(LocationOffset);
	
	//몽타주 에디터인지, 아닌지에 따라 회전 오프셋 설정 후, 에디터에서 지정한 Offset과 
	if (!IsValid(MeshComp->AnimClass))
	{
		FRotator CharacterRotationOffset = FRotator(0.f, 90.f, 0.f);
		ShapeTransform.SetRotation((ShapeTransform.Rotator() + CharacterRotationOffset).Quaternion());
		ShapeTransform.AddToTranslation(FVector(LocalOffset.Y, LocalOffset.X, LocalOffset.Z));
	}
	else
	{
		ShapeTransform.AddToTranslation(LocalOffset);
	}
	
	//Shape 생성
	FCollisionShape Shape;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MeshComp->GetOwner());
	TArray<FHitResult> Results;
	
	if (AttackCollisionShape.ShapeType == EShape::Box)
	{
		Shape = FCollisionShape::MakeBox(FVector(AttackCollisionShape.HalfExtentX, AttackCollisionShape.HalfExtentY, AttackCollisionShape.HalfExtentZ));
		
		UQVSystemLibrary::DrawDebugBox(MeshComp->GetWorld(), ShapeTransform.GetLocation(),
			FVector(AttackCollisionShape.HalfExtentX, AttackCollisionShape.HalfExtentY, AttackCollisionShape.HalfExtentZ), FColor::Red, FRotator(), 3.f);
	}
	else if (AttackCollisionShape.ShapeType == EShape::Capsule)
	{
		Shape = FCollisionShape::MakeCapsule(AttackCollisionShape.CapsuleRadius, AttackCollisionShape.HalfHeight);
	
		UQVSystemLibrary::DrawDebugCapsule(MeshComp->GetWorld(), ShapeTransform.GetLocation(), 
			AttackCollisionShape.HalfHeight, AttackCollisionShape.CapsuleRadius, FRotator::ZeroRotator, FColor::Red, 3.f);	
	}
	else if (AttackCollisionShape.ShapeType == EShape::Sphere)
	{
		Shape = FCollisionShape::MakeSphere(AttackCollisionShape.SphereRadius);
		
		UQVSystemLibrary::DrawDebugSphere(MeshComp->GetWorld(), ShapeTransform.GetLocation(), AttackCollisionShape.SphereRadius, 12, FColor::Red, 3.f);
	}
	
	bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(Results, ShapeTransform.GetLocation(), ShapeTransform.GetLocation(), FQuat::Identity, ECC_Pawn, Shape, Params);
	TArray<AActor*> ResultActors;
	
	if (bHit)
	{
		for (int i = 0; i < Results.Num(); ++i)
		{
			ACharacter* HitCharacter = Cast<ACharacter>(Results[i].GetActor());
			if (HitCharacter != nullptr)
			{
				if (!ResultActors.Contains(HitCharacter))
				{
					OutHitLocations.Add(HitCharacter->GetActorLocation());
				}
				ResultActors.AddUnique(HitCharacter);
			}
		}

		return ResultActors;
	}

	return TArray<AActor*>();
}


