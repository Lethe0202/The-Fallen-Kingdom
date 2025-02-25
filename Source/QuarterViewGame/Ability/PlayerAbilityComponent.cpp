// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilityComponent.h"
#include "BaseAbility.h"
#include "InventoryComponent.h"
#include "../Character/BaseCharacter.h"
#include "../Character/Component/ActionComponent.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/DataTableManager.h"
#include "../Manager/AnimationManager.h"
#include "../Manager/SaveManager.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"

UPlayerAbilityComponent::UPlayerAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(AbilityInputMappingContext, 1);
	}
	
	AbilityDecalActor = GetWorld()->SpawnActor<ADecalActor>(AbilityDecalActorClass);
	AbilityDecalComponent = AbilityDecalActor->GetComponentByClass<UDecalComponent>();
	AbilityDecalComponent->SetVisibility(false);
	
	InitPlayerInputComponent(PlayerController->InputComponent);
}

void UPlayerAbilityComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance != nullptr)
	{
		ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
		UDataTableManager* DataTableManager = GameInstance->GetDataTableManager();

		AbilityDash = NewObject<UBaseAbility>(this, AbilityDashClass);
		
		const FAbilityDataTableRow* AbilityStaticData = DataTableManager->GetAbilityStaticData(OwnerCharacter->GetCharacterID(), AbilityDash->GetAbilityID());
		if (AbilityStaticData != nullptr)
		{
			AbilityDash->InitStaticData(*AbilityStaticData);
		}
	}
}

void UPlayerAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateAbilityDecalLocation();
}

void UPlayerAbilityComponent::InitPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerEnhancedInputComponent != nullptr)
	{
		for (int32 i = 0; i < AbilityInputActionList.Num(); ++i)
		{
			if (AbilityList.IsValidIndex(i))
			{
				EAbilityActivationType AbilityActivationType = AbilityList[i]->GetAbilityActivationType();

				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[i], ETriggerEvent::Started, this, &UPlayerAbilityComponent::Input_AbilityStarted, i);
				
				if (AbilityActivationType == EAbilityActivationType::Hold || AbilityActivationType == EAbilityActivationType::Combo)
				{
					PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[i], ETriggerEvent::Triggered, this, &UPlayerAbilityComponent::Input_AbilityTriggered, i);
					PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[i], ETriggerEvent::Completed, this, &UPlayerAbilityComponent::Input_AbilityInputReleased, i);
					PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[i], ETriggerEvent::Canceled, this, &UPlayerAbilityComponent::Input_AbilityInputReleased, i);
				}
			}
		}
	}
	
	PlayerEnhancedInputComponent->BindAction(ActivateWaitingAbilityInputAction, ETriggerEvent::Started, this, &UPlayerAbilityComponent::Input_LeftMouseClick);
}

void UPlayerAbilityComponent::ActivateAbility(const int32 index)
{
	Super::ActivateAbility(index);
	
	AbilityDecalComponent->SetVisibility(false);
	PlayerEnhancedInputComponent->RemoveBindingByHandle(MouseClickHandle);	
	
	if (AbilityList.IsValidIndex(CurrentWaitingAbilityIndex) && AbilityList[CurrentWaitingAbilityIndex]->GetAbilityStaticData().AbilityActivationType == EAbilityActivationType::Combo)
	{
		CheckNextCombo();
	}
}

void UPlayerAbilityComponent::HandleAbilitySlotChange(UInputAction* Slot1, UInputAction* Slot2)
{
	if (Slot1 == nullptr && Slot2 == nullptr)
	{
		return;
	}
	
	if (Slot2 == nullptr)
	{
		RemoveAbilitySlot(Slot1);
	}
	else
	{
		int32 AbilityIndex1 = AbilityInputActionList.IndexOfByKey(Slot1);
		int32 AbilityIndex2 = AbilityInputActionList.IndexOfByKey(Slot2);
		AbilityList.Swap(AbilityIndex1, AbilityIndex2);
		
		//Delegate Clear
		if (AbilityList[AbilityIndex1] != nullptr && AbilityList[AbilityIndex1]->OnUpdateAbilityCooling.IsBound())
		{
			AbilityList[AbilityIndex1]->OnUpdateAbilityCooling.Clear();
		}
		
		if (AbilityList[AbilityIndex2] != nullptr && AbilityList[AbilityIndex2]->OnUpdateAbilityCooling.IsBound())
		{
			AbilityList[AbilityIndex2]->OnUpdateAbilityCooling.Clear();
		}
		
		if (AbilityList[AbilityIndex1] != nullptr && AbilityList[AbilityIndex1]->OnUpdateAbilityChainCooling.IsBound())
		{
			AbilityList[AbilityIndex1]->OnUpdateAbilityChainCooling.Clear();
		}
		
		if (AbilityList[AbilityIndex2] != nullptr && AbilityList[AbilityIndex2]->OnUpdateAbilityChainCooling.IsBound())
		{
			AbilityList[AbilityIndex2]->OnUpdateAbilityChainCooling.Clear();
		}

		//UI Update
		for (int i = 0; i < AbilityList.Num(); ++i)
		{
			if (AbilityList[i] == AbilityList[AbilityIndex1] || AbilityList[i] == AbilityList[AbilityIndex2])
			{
				OnAbilitySlotUpdate.Broadcast(AbilityInputActionList[i], this);
			}
		}
		
		RebindInputAction(AbilityIndex1);
		RebindInputAction(AbilityIndex2);
	}
}

void UPlayerAbilityComponent::HandleRegisterAbilitySlot(UInputAction* Slot, const FString AbilityID)
{
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance != nullptr)
	{
		ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
		UDataTableManager* DataTableManager = GameInstance->GetDataTableManager();

		const FAbilityDataTableRow* AbilityStaticData = DataTableManager->GetAbilityStaticData(FName(), *AbilityID);
		if (AbilityStaticData != nullptr && AbilityStaticData->AbilityClassRef != nullptr)
		{
			int32 AbilityIndex = AbilityInputActionList.IndexOfByKey(Slot);		
			
			if (AbilityIndex != -1)
			{
				int i = 0;
				for(i; i < AbilityList.Num(); ++i)
				{
					if (AbilityList[i] != nullptr && AbilityList[i]->IsA(AbilityStaticData->AbilityClassRef.LoadSynchronous()))
					{
						AbilityList[AbilityIndex] = AbilityList[i];
						break;
					}
				}

				if (i == AbilityList.Num())
				{
					AbilityList[AbilityIndex] = NewObject<UBaseAbility>(this, AbilityStaticData->AbilityClassRef.LoadSynchronous());
					AbilityList[AbilityIndex]->InitStaticData(*AbilityStaticData);
				}

				OnAbilitySlotUpdate.Broadcast(Slot, this);
				RebindInputAction(AbilityIndex);
			}			
		}
	}
}

void UPlayerAbilityComponent::Input_AbilityStarted(const FInputActionValue& Value, const int32 index)
{
	if (!AbilityList.IsValidIndex(index) || AbilityList[index] == nullptr)
	{
		return;
	}

	if (AbilityList[index]->GetAbilityRuntimeData().RemainingCooldown > 0)
	{
		return;
	}
	
	UBaseAbility* Ability = AbilityList[index];
	
	switch (Ability->GetAbilityActivationType())
	{
		case EAbilityActivationType::Instant:
			ActivateAbility(index);
			break;
		
		case EAbilityActivationType::Hold:
			ActivateAbility(index);
			break;
			
		case EAbilityActivationType::Combo:
			ActivateAbility(index);
			break;
			
		case EAbilityActivationType::Spot:
			ToggleSpotDecal(index);
			break;
			
		case EAbilityActivationType::Chain:
			ActivateAbility(index);
			break;
	}
}

void UPlayerAbilityComponent::Input_AbilityTriggered(const FInputActionValue& Value, const int32 index)
{
	if (!AbilityList.IsValidIndex(index) || AbilityList[index] == nullptr)
	{
		return;
	}
	
	const FAbilityStaticData& AbilityStaticData = AbilityList[index]->GetAbilityStaticData();
	FAbilityRuntimeData& AbilityRumtimeData = AbilityList[index]->GetAbilityRuntimeData();
	
	if (AbilityRumtimeData.bIsActuated == false)
	{
		return;
	}
	
	switch (AbilityList[index]->GetAbilityActivationType())
	{
	case EAbilityActivationType::Hold:
		HoldAbilityTriggered(index, AbilityStaticData, AbilityRumtimeData);
		break;
		
	case EAbilityActivationType::Combo:
		CheckNextCombo();
		break;
	}
}

void UPlayerAbilityComponent::Input_AbilityInputReleased(const FInputActionValue& Value, const int32 index)
{
	if (!AbilityList.IsValidIndex(index) || AbilityList[index] == nullptr)
	{
		return;
	}
	
	FAbilityRuntimeData& AbilityRumtimeData = AbilityList[index]->GetAbilityRuntimeData();
	
	if (AbilityRumtimeData.bIsActuated == true)
	{
		InputReleased(index);
	}
}

void UPlayerAbilityComponent::Input_LeftMouseClick(const FInputActionValue& Value)
{
	if (CurrentWaitingAbilityIndex != -1)
	{
		ActivateAbility(CurrentWaitingAbilityIndex);
	}
}

void UPlayerAbilityComponent::Input_DashStarted(const FInputActionValue& Value)
{
	if (!ActionComponent->CanChangeState(ECharacterStateType::Attack))
	{
		return;
	}

	AbilityDash->OnAbilityStarted.Clear();
	AbilityDash->OnAbilityEnded.Clear();
	AbilityDash->OnAbilityStarted.AddDynamic(this, &UPlayerAbilityComponent::HandleAbilityStarted);
	AbilityDash->OnAbilityEnded.AddDynamic(this, &UPlayerAbilityComponent::HandleAbilityEnded);

	AbilityDash->ActivateAbility();
}

void UPlayerAbilityComponent::InputReleased(const int32 index)
{
	const FAbilityStaticData& AbilityStaticData = AbilityList[index]->GetAbilityStaticData();
	FAbilityRuntimeData& AbilityRumtimeData = AbilityList[index]->GetAbilityRuntimeData();

	float HoldingTime = AbilityRumtimeData.HoldingRuntimeData.CurrentHoldTime;
	
	if (AbilityStaticData.HoldData.MinPerfectZone < HoldingTime && HoldingTime < AbilityStaticData.HoldData.MaxPerfectZone)
	{
		AbilityRumtimeData.HoldingRuntimeData.bPerfectZone = true;
	}
	
	OnUpdateAbilityProgress.Broadcast(-1.f, -1.f, -1.f, -1.f);

	AbilityList[index]->ReleasedInput();
	AbilityRumtimeData.bIsActuated = false;
}

void UPlayerAbilityComponent::HoldAbilityTriggered(const int32 index, const FAbilityStaticData& AbilityStaticData, FAbilityRuntimeData& AbilityRumtimeData)
{
	const float ElapsedTime = GetWorld()->GetTimeSeconds() - AbilityRumtimeData.LastActivationTime;
	AbilityRumtimeData.HoldingRuntimeData.CurrentHoldTime = ElapsedTime;
	const float Percent = FMath::Clamp(ElapsedTime / AbilityStaticData.HoldData.MaxHoldTime, 0.f, 1.f);
	
	const float PerfectMin = FMath::Clamp(AbilityStaticData.HoldData.MinPerfectZone / AbilityStaticData.HoldData.MaxHoldTime, 0.f, 1.f);
	const float PerfectMax = FMath::Clamp(AbilityStaticData.HoldData.MaxPerfectZone / AbilityStaticData.HoldData.MaxHoldTime, 0.f, 1.f);
	
	OnUpdateAbilityProgress.Broadcast(Percent, AbilityRumtimeData.HoldingRuntimeData.CurrentHoldTime, PerfectMin, PerfectMax);
	
	if (AbilityRumtimeData.HoldingRuntimeData.CurrentHoldTime >= AbilityStaticData.HoldData.MaxHoldTime)
	{
		InputReleased(index);
	}
}

void UPlayerAbilityComponent::CheckNextCombo()
{
	if (bEnableComboInput)
	{
		SetInputNextCombo(true);		
	}
}

void UPlayerAbilityComponent::ToggleSpotDecal(const int32 index)
{
	if (AbilityDecalComponent->IsVisible())
	{
		AbilityDecalComponent->SetVisibility(false);
		CurrentWaitingAbilityIndex = -1;
		PlayerEnhancedInputComponent->RemoveBindingByHandle(MouseClickHandle);	
	}
	else
	{
		float AbilityRange = AbilityList[index]->GetAbilityStaticData().SpotData.Radius;
		AbilityDecalComponent->DecalSize = FVector(AbilityRange, AbilityRange, AbilityRange);

		CurrentWaitingAbilityIndex = index;
		AbilityDecalComponent->SetVisibility(true);

		MouseClickHandle = PlayerEnhancedInputComponent->BindAction(ActivateWaitingAbilityInputAction, ETriggerEvent::Started,
			this, &UPlayerAbilityComponent::Input_LeftMouseClick).GetHandle();
	}
}

void UPlayerAbilityComponent::UpdateAbilityDecalLocation()
{
	if (!AbilityDecalComponent->IsVisible())
	{
		return;
	}

	if (AbilityDecalActor != nullptr && AbilityDecalComponent != nullptr && CurrentWaitingAbilityIndex >= 0)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
		{
			FHitResult HitResult;
			bool bHit = PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

			if (bHit)
			{
				FVector OwnActorLocation = GetOwner()->GetActorLocation();
				FVector MouseCursorWorldLocation = HitResult.Location;

				//ĳ���� -> ���帶�콺��ġ ���⺤��
				FVector DirectionVector2D = (MouseCursorWorldLocation - OwnActorLocation).GetSafeNormal2D();

				float OwnToMouseDistnace= FVector::DistXY(OwnActorLocation, MouseCursorWorldLocation);
				float AbilityDistance = AbilityList[CurrentWaitingAbilityIndex]->GetAbilityStaticData().SpotData.Distance;

				//��Į ��� �ִ� �Ÿ� üũ		
				if (OwnToMouseDistnace >= AbilityDistance)
				{
					FVector DecalActorLocation2D = OwnActorLocation + (DirectionVector2D * AbilityDistance);
					FVector SpotLocation = FVector(DecalActorLocation2D.X, DecalActorLocation2D.Y, MouseCursorWorldLocation.Z);

					AbilityDecalActor->SetActorLocation(SpotLocation);
					AbilityList[CurrentWaitingAbilityIndex]->GetAbilityRuntimeData().SpotRumtimeData.SpotLocation = SpotLocation;
				}
				else
				{
					AbilityList[CurrentWaitingAbilityIndex]->GetAbilityRuntimeData().SpotRumtimeData.SpotLocation = MouseCursorWorldLocation;
					AbilityDecalActor->SetActorLocation(MouseCursorWorldLocation);
				}						
			}
		}
	}
}

void UPlayerAbilityComponent::RemoveAbilitySlot(UInputAction* InputActionSlot)
{
	int32 AbilityIndex = AbilityInputActionList.IndexOfByKey(InputActionSlot);
	
	if (AbilityIndex != -1)
	{
		AbilityList[AbilityIndex]->OnUpdateAbilityChainCooling.Clear();
		AbilityList[AbilityIndex]->OnUpdateAbilityCooling.Clear();

		AbilityList[AbilityIndex] = nullptr;

		OnAbilitySlotUpdate.Broadcast(InputActionSlot, this);
		RebindInputAction(AbilityIndex);
	}
}

void UPlayerAbilityComponent::RebindInputAction(int32 AbilityIndex)
{
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	UInputComponent* PlayerInputComponent = OwnerCharacter->GetController()->InputComponent;

	PlayerEnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	if (PlayerEnhancedInputComponent != nullptr)
	{
		if (AbilityList.IsValidIndex(AbilityIndex) && AbilityList[AbilityIndex] != nullptr)
		{
			EAbilityActivationType AbilityActivationType = AbilityList[AbilityIndex]->GetAbilityActivationType();

			if (AbilityActivationType == EAbilityActivationType::Hold || AbilityActivationType == EAbilityActivationType::Combo)
			{
				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Triggered, this, &UPlayerAbilityComponent::Input_AbilityTriggered, AbilityIndex);
				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Completed, this, &UPlayerAbilityComponent::Input_AbilityInputReleased, AbilityIndex);
				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Canceled, this, &UPlayerAbilityComponent::Input_AbilityInputReleased, AbilityIndex);
			}
			else
			{
				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Triggered, nullptr, FName());
				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Completed, nullptr, FName());
				PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Canceled, nullptr, FName());
			}
		}
		else
		{
			PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Triggered, nullptr, FName());
			PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Completed, nullptr, FName());
			PlayerEnhancedInputComponent->BindAction(AbilityInputActionList[AbilityIndex], ETriggerEvent::Canceled, nullptr, FName());
		}
	}

	//EnhancedInputComponent->BindAction(ActivateWaitingAbilityInputAction, ETriggerEvent::Started, this, &UPlayerAbilityComponent::Input_LeftMouseClick);
}

UBaseAbility* UPlayerAbilityComponent::GetAbilityFromInputAction(const UInputAction* InputAction) const
{
	int index = AbilityInputActionList.IndexOfByKey(InputAction);

	if (index != -1 && AbilityList.IsValidIndex(index))
	{
		return AbilityList[index];
	}

	return nullptr;
}

const FString UPlayerAbilityComponent::GetAbilityInputKeyFromInputAction(const UInputAction* InputAction) const
{
	FString KeyString = " ";

	const TArray<FEnhancedActionKeyMapping>& AbilityInputMappings = AbilityInputMappingContext->GetMappings();

	for(const FEnhancedActionKeyMapping& AbilityInputMapping : AbilityInputMappings)
	{
		if (AbilityInputMapping.Action == InputAction)
		{
			KeyString = AbilityInputMapping.Key.ToString();
			break;
		}
	}

	return KeyString;
}

void UPlayerAbilityComponent::LoadData(USaveGameDataBase& LoadData)
{
	UAbilitySlotSaveData* AbilitySlotSaveData = Cast<UAbilitySlotSaveData>(&LoadData);
	TArray<TSubclassOf<UBaseAbility>> AbilityClassList = AbilitySlotSaveData->AbilitySlotClassList;
	
	UQuarterViewGameInstance* GameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance());
	
	if (GameInstance != nullptr)
	{
		ABaseCharacter* OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
		UDataTableManager* DataTableManager = GameInstance->GetDataTableManager();
		
		for (int32 i = 0; i < AbilityClassList.Num(); ++i)
		{
			if (AbilityDash->GetClass() == AbilityClassList[i])
			{
				continue;
			}
			
			if (AbilityList.IsValidIndex(i))
			{
				UBaseAbility* Ability = NewObject<UBaseAbility>(this, AbilityClassList[i]);
				AbilityList[i] = Ability;
				
				const FAbilityDataTableRow* AbilityStaticData = DataTableManager->GetAbilityStaticData(OwnerCharacter->GetCharacterID(), Ability->GetAbilityID());
				if (AbilityStaticData != nullptr)
				{
					Ability->InitStaticData(*AbilityStaticData);
				}

				RebindInputAction(i);
				
				OnAbilitySlotUpdate.Broadcast(AbilityInputActionList[i], this);
			}
		}
	}
}

void UPlayerAbilityComponent::SaveData(USaveGameDataBase*& SaveData)
{
	SaveData = NewObject<UAbilitySlotSaveData>();
	UAbilitySlotSaveData* AbilitySlotSaveData = Cast<UAbilitySlotSaveData>(SaveData);
	
	for (int i = 0; i < AbilityList.Num(); i++)
	{
		AbilitySlotSaveData->AbilitySlotClassList.Add(AbilityList[i]->GetClass());
	}
}

ESaveDataType UPlayerAbilityComponent::SaveDataType()
{
	return ESaveDataType::AbilitySlot;
}
