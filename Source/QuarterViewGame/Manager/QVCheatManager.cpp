// Fill out your copyright notice in the Description page of Project Settings.


#include "QVCheatManager.h"

TAutoConsoleVariable<int32> UQVCheatManager::CVarDrawDebugCollision(
    TEXT("DebugManager.DrawDebugCollision"),
    0,
    TEXT("Toggles DrawDebugCollision system. 0:Disabled, 1:Enabled"),
    ECVF_Default
    );

TAutoConsoleVariable<int32> UQVCheatManager::CVarAbilityCoolingTimeZero(
        TEXT("DebugManager.AbilityCoolingTime"),
        0,
        TEXT("Toggles DrawDebugCollision system. 0:CoolingTime Enable, 1:CoolingTime Disable"),
        ECVF_Default
    );