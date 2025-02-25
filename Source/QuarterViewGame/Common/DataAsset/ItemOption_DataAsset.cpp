// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemOption_DataAsset.h"

const TArray<TSubclassOf<UUserWidget>>& UItemOption_DataAsset::GetButtonActionWidget() const
{
	return ItemButtonActions;
}
