// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarterViewGame/Interface/SaveableInterface.h"
#include "CharacterStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealtChanged, float, NewHealthPercent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class QUARTERVIEWGAME_API UCharacterStatusComponent : public UActorComponent, public ISaveableInterface
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UCharacterStatusComponent();
	
	void TakeDamage(float DamageValue, AActor* DamageCauser);
	
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	
	float CalculateHitDirection(FVector TargetLocation);
	
public:
	/* ISaveableInterface */
	virtual void LoadData(USaveGameDataBase& LoadData) override;
	virtual void SaveData(/*out*/USaveGameDataBase*& SaveData) override;
	virtual ESaveDataType SaveDataType() override;
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnHealtChanged OnHealthChanged;
	
private:
	float MaxHP;
	float CurrentHP;

	UPROPERTY(EditAnywhere)
	bool bActivate = true;
};
