// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuarterViewGame/Inventory/InventoryComponent.h"
#include "DropStaticItem.generated.h"

UCLASS()
class QUARTERVIEWGAME_API ADropStaticItem : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADropStaticItem();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitDropItem(const FName& InItemID);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EItemType ItemType;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName ItemID;
};
