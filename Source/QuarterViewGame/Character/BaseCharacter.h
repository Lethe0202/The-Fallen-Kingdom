// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCharacterStatusComponent;
class UActionComponent;
class UAbilityComponent;
class ICharacterState;

UENUM(BlueprintType)
enum class ETeamType : uint8
{
	PlayerTeam,
	EnemyTeam,
};

UCLASS(BlueprintType, Abstract)
class QUARTERVIEWGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void PrimaryAttack();
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void DeadCharacter(AActor* DamageCauser);

	/**
	 * 캐릭터 회전
	 * TargetRotator : ZeroRotator이면 마우스 포인터로 회전
	 */
	void RotateLerpCharacter(FRotator TargetRotator = FRotator(), float LerpTime = 0.25f);

protected:
	virtual void BeginPlay() override;
	
public:
	virtual UAbilityComponent* GetAbilityComponent();
	const ETeamType& GetTeamType() const { return TeamType; }
	UActionComponent* GetActionComponent() const { return ActionComponent ? ActionComponent : nullptr; }
	UCharacterStatusComponent* GetCharacterStatusComponent() const{	return CharacterStatusComponent ? CharacterStatusComponent : nullptr; }
	const FName& GetCharacterID() const { return CharacterID; }
	virtual const FText GetCharacterDisplayName() const;
	virtual const FVector GetMouseCursorWorldLocation() const;

protected:
	UFUNCTION()
	void OnEndAction();

	UFUNCTION()
	void OnRotationTimer();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Base", meta = (AllowPrivateAccess = "true"))
	FName CharacterID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character | Base", meta = (AllowPrivateAccess = "true"))
	ETeamType TeamType;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Base", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterStatusComponent> CharacterStatusComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character | Base", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UActionComponent> ActionComponent;
	
private:
	//캐릭터 회전관련변수
	FTimerHandle RotationTimerHandle;
	float RotateStartTime;
	float RotationTime;
	FRotator StartRotation;
	FRotator TargetRotation;
};
