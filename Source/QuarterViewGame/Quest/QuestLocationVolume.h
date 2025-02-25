// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestLogComponent.h"
#include "GameFramework/Actor.h"
#include "QuestLocationVolume.generated.h"

class USphereComponent;

UCLASS()
class QUARTERVIEWGAME_API AQuestLocationVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestLocationVolume();

	void SetLocationVolumeID(const FName& VolumeID);
	void SetActive(bool bActive);
	const FName& GetLocationID() const { return LocationID; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent> DecalComponent;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FName LocationID;
};
