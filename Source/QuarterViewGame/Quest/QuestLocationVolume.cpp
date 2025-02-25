// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestLocationVolume.h"

#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
AQuestLocationVolume::AQuestLocationVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	RootComponent = SphereComponent;

	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	DecalComponent->SetupAttachment(SphereComponent);
}

void AQuestLocationVolume::SetLocationVolumeID(const FName& VolumeID)
{
	LocationID = VolumeID;
}

void AQuestLocationVolume::SetActive(bool bActive)
{
	if (bActive)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		DecalComponent->SetVisibility(true);
	}
	else
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DecalComponent->SetVisibility(false);
	}
}

// Called when the game starts or when spawned
void AQuestLocationVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AQuestLocationVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

