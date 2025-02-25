// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Chaos/Tetrahedron.h"
#include "ItemMenuPanelWidget.generated.h"

class UItemOption_DataAsset;
class UInventoryPanelWidget;

UCLASS()
class QUARTERVIEWGAME_API UItemMenuPanelWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Init(UInventoryPanelWidget* InventoryPanelWidget, int32 Index, const FText& InItemName, EItemType InItemType);
	
protected:
	//아이템 타입에 따른 옵션 Map
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EItemType, TObjectPtr<UItemOption_DataAsset>> OptionMap;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TWeakObjectPtr<UInventoryPanelWidget> ParentPanelWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 ItemIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EItemType ItemType;
};