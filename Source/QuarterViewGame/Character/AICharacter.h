// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "AICharacter.generated.h"

class UBehaviorTree; 
class UWidgetComponent;

UCLASS()
class QUARTERVIEWGAME_API AAICharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AAICharacter();

	virtual void PrimaryAttack() override;
	virtual void DeadCharacter(AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

public:
	UBehaviorTree* const GetBT() const { return (BT != nullptr ? BT : nullptr); }
	virtual UAbilityComponent* GetAbilityComponent() override;
	virtual const FText GetCharacterDisplayName() const override;
	virtual const FVector GetMouseCursorWorldLocation() const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character | AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* BT;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Ability", meta = (AllowPrivateAccess = "true"))
	UAbilityComponent* AbilityComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | UI", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	FText CharacterDisplayName;
};
