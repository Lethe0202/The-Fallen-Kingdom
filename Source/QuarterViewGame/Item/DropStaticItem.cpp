// Fill out your copyright notice in the Description page of Project Settings.


#include "DropStaticItem.h"

#include "ItemManager.h"
#include "QuarterViewGameInstance.h"

// Sets default values
ADropStaticItem::ADropStaticItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADropStaticItem::BeginPlay()
{
	Super::BeginPlay();
}

void ADropStaticItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (ItemID.IsValid())
	{
		InitDropItem(ItemID);
	}
}

void ADropStaticItem::InitDropItem_Implementation(const FName& InItemID)
{
	ItemID = InItemID;
}

// Called every frame
void ADropStaticItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

