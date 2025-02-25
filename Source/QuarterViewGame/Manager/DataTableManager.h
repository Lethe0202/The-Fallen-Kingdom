// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "../Common/AbilityStructs.h"
#include "DataTableManager.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UDataTableManager : public UObject
{
	GENERATED_BODY()
	
public:
	const FCharacterStatusaRow* GetCharacterPrimaryStatus(const FName& CharacterID) const;
	const FAbilityDataTableRow* GetAbilityStaticData(const FName& CharacterID, const FName& AbilityID) const;
	
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> CharacterStatusDataTable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> AbilityStaticDataTable;
};
