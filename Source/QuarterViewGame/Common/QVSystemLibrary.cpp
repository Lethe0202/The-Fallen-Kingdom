// Fill out your copyright notice in the Description page of Project Settings.


#include "QVSystemLibrary.h"

#include "../Manager/QVCheatManager.h"
#include "Kismet/KismetSystemLibrary.h"

void UQVSystemLibrary::DrawDebugBox(const UObject* WorldContextObject, FVector const Center, FVector Extent, FLinearColor Color, const FRotator Rotation, float LifeTime, float Thickness)
{
	if (UQVCheatManager::IsDrawDebugCollisionEnabled())
	{
		UKismetSystemLibrary::DrawDebugBox(WorldContextObject, Center, Extent, Color, Rotation, LifeTime, Thickness);
	}
}

void UQVSystemLibrary::DrawDebugSphere(const UObject* WorldContextObject, FVector const Center, float Radius, int32 Segments, FLinearColor Color, float LifeTime, float Thickness)
{
	if (UQVCheatManager::IsDrawDebugCollisionEnabled())
	{
		UKismetSystemLibrary::DrawDebugSphere(WorldContextObject, Center, Radius, Segments, Color, LifeTime, Thickness);
	}
}

void UQVSystemLibrary::DrawDebugCapsule(const UObject* WorldContextObject, FVector const Center, float HalfHeight, float Radius, const FRotator Rotation, FLinearColor Color, float LifeTime, float Thickness)
{
	if (UQVCheatManager::IsDrawDebugCollisionEnabled())
	{
		UKismetSystemLibrary::DrawDebugCapsule(WorldContextObject, Center, HalfHeight, Radius, Rotation, Color, LifeTime, Thickness);
	}
}