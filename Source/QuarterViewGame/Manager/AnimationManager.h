// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "../Common/QVEnumHeader.h"
#include "AnimationManager.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UAnimationManager : public UObject
{
	GENERATED_BODY()

public:
	//스킬 애니메이션 모음 FAbilityAnimRow 구조체 반환
	const FAbilityAnimRow* GetAbilityMontage(const FName& CharacterID, const FName& AbilityID) const;

	//일반적인 히트, Dead 몽타주 반환
	UAnimMontage* GetActionAnimMontage(const FName& CharacterID, const EActionAnimationType ActionAnimType) const;
	
	UAnimMontage* LoadMontage(const TSoftObjectPtr<UAnimMontage>& MontagePtr) const;

private:
	//스킬 애니메이션 DataTable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* CharacterAbilityAnimDataTable;

	//액션 애니메이션 DataTable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UDataTable* CharacterActionAnimDataTable;
};
