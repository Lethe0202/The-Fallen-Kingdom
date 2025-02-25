// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemOption_DataAsset.generated.h"

class UUserWidget;

/*
 * 아이템에 따라 생성되어야하는 옵션 WidgetClass DataAsset
 */
UCLASS(BlueprintType)
class QUARTERVIEWGAME_API UItemOption_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	const TArray<TSubclassOf<UUserWidget>>& GetButtonActionWidget() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSubclassOf<UUserWidget>> ItemButtonActions;
};
