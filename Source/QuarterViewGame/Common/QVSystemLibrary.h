// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "QVSystemLibrary.generated.h"

/**
 * 기존 엔진기능을 재활용하여 프로젝트에 맞게 코드 수정
 * 게임에 전반적으로 사용되는 기능 라이브러리
 */
UCLASS()
class QUARTERVIEWGAME_API UQVSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static void DrawDebugBox(const UObject* WorldContextObject, FVector const Center, FVector Extent, FLinearColor Color, const FRotator Rotation, float LifeTime=0, float Thickness=0);
	
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static void DrawDebugSphere(const UObject* WorldContextObject, FVector const Center, float Radius, int32 Segments, FLinearColor Color, float LifeTime=0, float Thicknes=0);
	
	UFUNCTION(BlueprintCallable, Category = "Debug", meta = (WorldContext = "WorldContextObject", DevelopmentOnly))
	static void DrawDebugCapsule(const UObject* WorldContextObject, FVector const Center, float HalfHeight, float Radius, const FRotator Rotation, FLinearColor Color, float LifeTime=0, float Thickness=0);
};
