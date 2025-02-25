#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Common/QVEnumHeader.h"
#include "AbilityStructs.h"
#include "../Inventory/InventoryComponent.h"
#include "../Quest/QuestLogComponent.h"
#include "QuarterViewGameDataTable.generated.h"

class ABaseCharacter;
class UTexture2D;
class UBaseAbility;
class UItemEffectBase;
class UAnimMontage;

/*
 * 게임에 전반적으로 사용되는 구조체
 */

USTRUCT(BlueprintType)
struct FCharacterStatusaRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHp;
};

USTRUCT(BlueprintType)
struct FAbilityAnimRow : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<TSoftObjectPtr<UAnimMontage>> AbilityMontages;
};

USTRUCT(BlueprintType)
struct FAbilityDataTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> IconTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseDamage = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BaseCooldown = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftClassPtr<UBaseAbility> AbilityClassRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UParticleSystem> HitParticleSystem;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EAbilityActivationType AbilityActivationType;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "AbilityActivationType == EAbilityActivationType::Hold"))
    FAbilityHoldStaticData HoldData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "AbilityActivationType == EAbilityActivationType::Spot"))
    FAbilitySpotStaticData SpotData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (EditCondition = "AbilityActivationType == EAbilityActivationType::Chain"))
    FAbilityChainStaticData ChainData;
};

USTRUCT(BlueprintType)
struct FActionAnimationTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FName CharacterID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    EActionAnimationType ActionAnimType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TSoftObjectPtr<UAnimMontage> AnimationMontage;
};

USTRUCT(BlueprintType)
struct FConsumptionItemRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> IconImage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UStaticMesh> StaticMesh;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TObjectPtr<UItemEffectBase> Effect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemQuantity;
};

USTRUCT(BlueprintType)
struct FQuestReward
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestRewardType QuestRewardType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestRewardType == EQuestRewardType::Item"))
    FName ItemID;
};

USTRUCT(BlueprintType)
struct FQuestChapterRow : public FTableRowBase
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText QuestChapterDisplayName;
};

USTRUCT(BlueprintType)
struct FQuestInfoRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText QuestInfoDisplayName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"))
    FText QuestDetail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> PreQuestArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FName> TaskInfoIDArray;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FQuestReward> QuestRewardArray;
};

USTRUCT(BlueprintType)
struct FQuestTaskInfo
{
    GENERATED_BODY()
    
};

USTRUCT(BlueprintType)
struct FQuestTaskInfo_Location : public FQuestTaskInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName LocationTriggerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ObjectiveLocation;
};

USTRUCT(BlueprintType)
struct FQuestTaskInfo_Interact : public FQuestTaskInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName InteractID;
};

USTRUCT(BlueprintType)
struct FQuestTaskInfo_Kill : public FQuestTaskInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName TargetID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity;
};

USTRUCT(BlueprintType)
struct FTaskObjectives
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestType QuestType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestType == EQuestType::Location"))
    FQuestTaskInfo_Location QuestTaskInfo_Location;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestType == EQuestType::Kill"))
    FQuestTaskInfo_Kill QuestTaskInfo_Kill;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "QuestType == EQuestType::Interact"))
    FQuestTaskInfo_Interact QuestTaskInfo_Interact;
};

USTRUCT(BlueprintType)
struct FQuestTaskRow : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText QuestTaskisplayName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FTaskObjectives> TaskObjectives;
};


USTRUCT(BlueprintType)
struct FItemDropInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType ItemType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DropRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DropQuantity;
};

USTRUCT(BlueprintType)
struct FItemDropTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FItemDropInfo> ItemDropArray;
};

USTRUCT(BlueprintType)
struct FWorldInfoTableRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> WorldMapTexture2D;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OrthoWidth;
};

