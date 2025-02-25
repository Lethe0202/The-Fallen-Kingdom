// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimationManager.h"

#include "../Common/QuarterViewGameDataTable.h"

const FAbilityAnimRow* UAnimationManager::GetAbilityMontage(const FName& CharacterID, const FName& AbilityID) const
{
    FString RowName = FString::Printf(TEXT("%s_%s"), *CharacterID.ToString(), *AbilityID.ToString());

    if (FAbilityAnimRow* AbilityAnimRow = CharacterAbilityAnimDataTable->FindRow<FAbilityAnimRow>(*RowName, TEXT("")))
    {
        return AbilityAnimRow;
    }
    
    return nullptr;
}

UAnimMontage* UAnimationManager::GetActionAnimMontage(const FName& CharacterID, const EActionAnimationType ActionAnimType) const
{
    //Enum -> string
    FString ActionnTypeString = UEnum::GetValueAsString(TEXT("QuarterViewGame.EActionAnimationType"), ActionAnimType);

    //CharacterID_EnumString -> RowName / 프로젝트만의 규칙으로 RowName 생성
    FString RowName = FString::Printf(TEXT("%s_%s"), *CharacterID.ToString(), *ActionnTypeString);

    if (FActionAnimationTableRow* ActionAnimRow = CharacterActionAnimDataTable->FindRow<FActionAnimationTableRow>(*RowName, TEXT("")))
    {
        return LoadMontage(ActionAnimRow->AnimationMontage);
    }

    return nullptr;
}

UAnimMontage* UAnimationManager::LoadMontage(const TSoftObjectPtr<UAnimMontage>& MontagePtr) const
{
    if (!MontagePtr.IsNull())
    {
        return MontagePtr.LoadSynchronous();
    }

    return nullptr;
}
