// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerAbilityComponent.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAbilityComponent;
class UPlayerAbilityComponent;
class UInventoryComponent;
class UItemQuickSlotComponent;
class UQuestLogComponent;
class USphereComponent;

//상호작용, 킬 등 플레이어와 무언가 교류가 있을경우 호출
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWorkObjectiveID, const FName&, ObjectiveID);

UCLASS()
class QUARTERVIEWGAME_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
public:
	FORCEINLINE virtual UAbilityComponent* GetAbilityComponent() override { return AbilityComponent; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	void Interaction();
	void InputMove();
	virtual void DeadCharacter(AActor* DamageCauser) override;
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION()
	void OnSuccessQuest(const FName& QuestID);
	
	UFUNCTION()
	void OnInteractCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnInteractCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnWorkObjectiveID OnWorkObjectiveID;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UPlayerAbilityComponent* AbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UItemQuickSlotComponent* ItemQuickSlotComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Ability, meta = (AllowPrivateAccess = "true"))
	UQuestLogComponent* QuestLogComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	USphereComponent* InteractionCollision;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	UPROPERTY()
	AActor* InteractableActor;
};
