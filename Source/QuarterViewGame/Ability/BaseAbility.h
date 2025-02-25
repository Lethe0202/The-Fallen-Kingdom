// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbilityComponent.h"
#include "../Common/AbilityStructs.h"
#include "../Common/QuarterViewGameDataTable.h"
#include "BaseAbility.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAbilityCoolingSignature, float, Percent, float, CoolingTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateAbilityChainCoolingSignature, float, Percent, float, CoolingTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityEnded);

class ACharacter;

UCLASS(Blueprintable, BlueprintType, Abstract)
class QUARTERVIEWGAME_API UBaseAbility : public UObject
{
	GENERATED_BODY()
	
public:
	void InitStaticData(const FAbilityDataTableRow& InAbilityStaticData);
	
	virtual void ActivateAbility();
	virtual void AbilityEffect(AActor* EffectTarget);
	virtual void AbilitySpawnHitSystem(FVector Location);
	virtual void AdditionalAction(int32 ActionIndex);
	virtual void ReleasedInput();

protected:
	virtual void ResetRumtimeData();

	virtual void StartAbilityCoolingTimer();
	void StartChainAbilityCoolingTimer();
	
	bool PlayMontageToIndex(int index);
	
	ACharacter* GetOwnCharacter() const { return Cast<ACharacter>(GetOuter()->GetOuter()); }
	
	UFUNCTION()
	void OnEndAbility(UAnimMontage* Montage, bool bInterrupted);
	
	//��ų ��Ÿ��
	UFUNCTION()
	void OnUpdateAbilityCoolingTimer();

	//ü�� ��Ÿ��
	UFUNCTION()
	void OnUpdateAbilityChainCoolingTimer();
	
public:
	const EAbilityActivationType& GetAbilityActivationType() const { return StaticData.AbilityActivationType; }
	const FName& GetAbilityID() const { return AbilityID; }
	const FAbilityStaticData& GetAbilityStaticData() const { return StaticData; }
	const FAbilityRuntimeData& GetAbilityRuntimeData() const { return RuntimeData; }
	FAbilityRuntimeData& GetAbilityRuntimeData() { return RuntimeData; }

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityStarted OnAbilityStarted;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityEnded OnAbilityEnded;

	FOnUpdateAbilityCoolingSignature OnUpdateAbilityCooling;
	FOnUpdateAbilityChainCoolingSignature OnUpdateAbilityChainCooling;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (AllowPrivaetAccess = "true"))
	FName AbilityID;

	// ���� ������ (�ʱ�ȭ �� ������� ����)
	UPROPERTY()
	FAbilityStaticData StaticData;

	// ��Ÿ�� ������ (���� �� �����)
	UPROPERTY()
	FAbilityRuntimeData RuntimeData;

	UPROPERTY()
	TArray<AActor*> AlreadyDamagedArray;

private:
	FTimerHandle CoolingTimerHandle;
	FTimerHandle ChainCoolingTimerHandle;

	TWeakObjectPtr<UAnimMontage> LastPlayMontage;
};

