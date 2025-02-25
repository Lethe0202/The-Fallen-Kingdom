// Fill out your copyright notice in the Description page of Project Settings.


#include "DataTableManager.h"

#include "../Common/QuarterViewGameDataTable.h"
#include "../Common/AbilityStructs.h"

const FCharacterStatusaRow* UDataTableManager::GetCharacterPrimaryStatus(const FName& CharacterID) const
{
	if (CharacterStatusDataTable)
	{
		FCharacterStatusaRow* CharacterStatusaRow = CharacterStatusDataTable->FindRow<FCharacterStatusaRow>(CharacterID, TEXT(""));

		return CharacterStatusaRow ? CharacterStatusaRow : nullptr;
	}

	return nullptr;
}

const FAbilityDataTableRow* UDataTableManager::GetAbilityStaticData(const FName& CharacterID, const FName& AbilityID) const
{
	if (AbilityStaticDataTable)
	{
		FString RowName;

		if (CharacterID.IsNone())
		{
			RowName = *AbilityID.ToString();
		}
		else
		{
			RowName = FString::Printf(TEXT("%s_%s"), *CharacterID.ToString(), *AbilityID.ToString());
		}
		
		FAbilityDataTableRow* AbilityStaticData = AbilityStaticDataTable->FindRow<FAbilityDataTableRow>(*RowName, TEXT(""));
		
		return AbilityStaticData ? AbilityStaticData : nullptr;
	}
	
	return nullptr;
}
