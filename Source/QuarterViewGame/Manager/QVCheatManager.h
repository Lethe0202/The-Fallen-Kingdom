// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "QVCheatManager.generated.h"

/**
 * 
 */
UCLASS()
class QUARTERVIEWGAME_API UQVCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static bool IsDrawDebugCollisionEnabled()
    {
        return CVarDrawDebugCollision.GetValueOnGameThread() != 0;
    }

    UFUNCTION(BlueprintCallable)
    static bool IsAbilityCoolingTimeZero()
    {
        return CVarAbilityCoolingTimeZero.GetValueOnGameThread() != 0;
    }
    
    UFUNCTION(BlueprintCallable)
    static void DrawDebugCollision(bool bDraw)
    {
        CVarDrawDebugCollision->Set(bDraw);
    }
    
    UFUNCTION(BlueprintCallable)
    static void SetAbilityCoolingTimeZero(bool bZero)
    {
        CVarAbilityCoolingTimeZero->Set(bZero);
    }
    
private:
    static TAutoConsoleVariable<int32> CVarDrawDebugCollision;
    static TAutoConsoleVariable<int32> CVarAbilityCoolingTimeZero;
};
    