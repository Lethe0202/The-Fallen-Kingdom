#pragma once

#include "CoreMinimal.h"
#include "QVEnumHeader.generated.h"

UENUM(BlueprintType)
enum class EActionAnimationType : uint8
{
    HitFront,
    HitBack,
    HitLeft,
    HitRight,
    Knockback,
    Dead,
};

UENUM(BlueprintType)
enum class EAbilityActivationType : uint8
{
	Instant,    // 일반
	Combo,
	Hold,
	Spot,		
	Chain,			
	Toggle
};

UENUM(BlueprintType)
enum class EQuestRewardType : uint8
{
	Exp,
	Gold,
	Item,
};
