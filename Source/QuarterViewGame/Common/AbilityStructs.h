#pragma once

#include "CoreMinimal.h"
#include "QVEnumHeader.h"
#include "AbilityStructs.generated.h"

class UMaterialInterface;
class UNiagaraSystem;
class UTexture2D;
class UParticleSystem;

/*
 * 스킬의 Static(정적), Dynamic(동적) 데이터 구조체
 */

USTRUCT(BlueprintType)
struct FAbilityHoldStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MaxHoldTime = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MinPerfectZone = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float MaxPerfectZone = 0.f;
};

USTRUCT(BlueprintType)
struct FAbilitySpotStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Radius = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Distance = 0.f;
};

USTRUCT(BlueprintType)
struct FAbilityChainStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    int32 MaxChainCnt = 0;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float AdditionalInputTime = 0.f;
};

//정적 데이터
USTRUCT(BlueprintType)
struct FAbilityStaticData
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability | Basic")
    FText AbilityName;

    UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Ability | Basic")
    FText Description;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability | Basic")
    float BaseDamage = 0.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability | Basic")
    float BaseCooldown = 0.f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability | Visual")
    TSoftObjectPtr<UTexture2D> IconTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UParticleSystem> HitParticleSystem;
    
    //스킬 타입 
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    EAbilityActivationType AbilityActivationType;

    //홀드
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "AbilityActivationType == EAbilityActivationType::Hold"))
    FAbilityHoldStaticData HoldData;
    
    //지점
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "AbilityActivationType == EAbilityActivationType::Spot"))
    FAbilitySpotStaticData SpotData;

    //체인
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "AbilityActivationType == EAbilityActivationType::Chain"))
    FAbilityChainStaticData ChainData;
};

/*
 * 동적 데이터
 */

//홀드
USTRUCT(BlueprintType)
struct FAbilityHoldingRuntimeData
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    float CurrentHoldTime = 0.f;

    UPROPERTY(Transient)
    bool bPerfectZone = true;

    void Reset()
    {
        CurrentHoldTime = 0.f;
        bPerfectZone = false;
    }
};

//체인
USTRUCT(BlueprintType)
struct FAbilityChainRumtimeData
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    int32 CurrentChainCnt = 0;

    UPROPERTY(Transient)
    float AdditionalInputRemainingTime = 0.f;

    void Reset()
    {
        CurrentChainCnt = 0;
        AdditionalInputRemainingTime = 0.f;
    }
};

//지점
USTRUCT(BlueprintType)
struct FAbilitySpotRumtimeData
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    FVector SpotLocation;

    void Reset()
    {
        SpotLocation = FVector::ZeroVector;
    }
};

USTRUCT(BlueprintType)
struct FAbilityRuntimeData
{
    GENERATED_BODY()

    UPROPERTY(Transient)
    float CurrentDamage;

    //input이 press중인지 체크
    UPROPERTY(Transient)
    bool bIsActuated = false;

    //AbilityEffect 호출횟수
    UPROPERTY(Transient)
    int32 CurrentEffectCount;

    UPROPERTY(Transient)
    float RemainingCooldown = 0.f;

    UPROPERTY(Transient)
    float LastActivationTime = 0.f;

    //홀딩 런타임 데이터
    UPROPERTY(Transient)
    FAbilityHoldingRuntimeData HoldingRuntimeData;

    //체인 런타임 데이터
    FAbilityChainRumtimeData ChainRumtimeData;

    //스팟 런타임 데이터
    FAbilitySpotRumtimeData SpotRumtimeData;

    void Reset()
    {
        CurrentDamage = 0.f;
        RemainingCooldown = 0.f;
        bIsActuated = false;
        CurrentEffectCount = 0;
        LastActivationTime = 0.f;
        HoldingRuntimeData.Reset();
        ChainRumtimeData.Reset();
        SpotRumtimeData.Reset();
    }
};