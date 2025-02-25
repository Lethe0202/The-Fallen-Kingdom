// Fill out your copyright notice in the Description page of Project Settings.

#include "QuarterViewPlayerController.h"
#include "../Character/PlayerCharacter.h"
#include "../UI/HUD/PlayerHUD.h"
#include "../Ability/AbilityComponent.h"
#include "../Ability/PlayerAbilityComponent.h"
#include "../Character/Component/CharacterStatusComponent.h"
#include "../Character/Component/ActionComponent.h"
#include "../Inventory/InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InputActionValue.h"
#include "QuarterViewGameInstance.h"
#include "UIManager.h"

#include "Framework/Application/SlateApplication.h"
#include "Layout/WidgetPath.h"
# include "Widgets/SWidget.h"
# include "Widgets/SViewport.h"
//#include "Widgets/SWindow.h"

AQuarterViewPlayerController::AQuarterViewPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AQuarterViewPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AQuarterViewPlayerController::OnSetDestinationStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AQuarterViewPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AQuarterViewPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AQuarterViewPlayerController::OnSetDestinationReleased);
		
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this, &AQuarterViewPlayerController::Input_OpenInventory);
		EnhancedInputComponent->BindAction(OpenAbilityPanelAction, ETriggerEvent::Started, this, &AQuarterViewPlayerController::Input_OpenAbilityPanel);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this, &AQuarterViewPlayerController::Input_Interaction);
		EnhancedInputComponent->BindAction(OpenQuestTabAction, ETriggerEvent::Started, this, &AQuarterViewPlayerController::Input_QuestTab);
		EnhancedInputComponent->BindAction(OpenWorldMpaAction, ETriggerEvent::Started, this, &AQuarterViewPlayerController::Input_OpenWorldMap);
	}
}

void AQuarterViewPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	if (HUDWidgetClass)
	{
		PlayerHUD = CreateWidget<UPlayerHUD>(this, HUDWidgetClass);
		
		if (PlayerHUD)
		{
			PlayerHUD->AddToViewport(0);
		}
		
		PlayerHUD->InitAbilitySlot(OwnCharacter->GetComponentByClass<UPlayerAbilityComponent>());
		PlayerHUD->InitItemQuickSlot(OwnCharacter->GetComponentByClass<UItemQuickSlotComponent>());
		PlayerHUD->InitHealthBar(OwnCharacter->GetComponentByClass<UCharacterStatusComponent>());
		PlayerHUD->InitInventoryPanel(OwnCharacter->GetComponentByClass<UInventoryComponent>());
		
		if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
		{
			if (UUIManager* UIManager = QuarterViewGameInstance->GetUIManager())
			{
				UIManager->RegisterPlayerHUD(PlayerHUD);
			}
		}
	}
}

void AQuarterViewPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	OwnCharacter = Cast<APlayerCharacter>(InPawn);
	OwnerActionComponent = OwnCharacter->GetActionComponent();
}

void AQuarterViewPlayerController::OnSetDestinationStarted()
{
	if (IsCursorOnUI())
	{
		return;
	}
	
	StopMovement();
}

void AQuarterViewPlayerController::OnSetDestinationTriggered()
{
	if (IsCursorOnUI())
	{
		return;
	}
	
	if (!OwnerActionComponent->CanMove())
	{
		return;
	}
	
	FollowTime += GetWorld()->GetDeltaSeconds();

	FHitResult Hit;
	bool bHitSuccessful = false;

	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true,Hit);
	
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
		
		OwnCharacter->InputMove();
	}
}

void AQuarterViewPlayerController::OnSetDestinationReleased()
{
	if (IsCursorOnUI())
	{
		return;
	}
	
	if (!OwnerActionComponent->CanMove())
	{
		return;
	}
	
	if (FollowTime <= ShortPressThreshold)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
	
	FollowTime = 0.f;
}

void AQuarterViewPlayerController::Input_OpenInventory(const FInputActionValue& Value)
{
	PlayerHUD->ToggleInventory();
}

void AQuarterViewPlayerController::Input_OpenAbilityPanel(const FInputActionValue& Value)
{
	PlayerHUD->ToggleAbilityPanel();
}

void AQuarterViewPlayerController::Input_Interaction(const FInputActionValue& Value)
{
	OwnCharacter->Interaction();
}

void AQuarterViewPlayerController::Input_QuestTab(const FInputActionValue& Value)
{
	PlayerHUD->ToggleQuestPanel();
}

void AQuarterViewPlayerController::Input_OpenWorldMap(const FInputActionValue& Value)
{
	PlayerHUD->ToggleWorldMap();
}

bool AQuarterViewPlayerController::IsCursorOnUI()
{
	TSharedPtr ViewPort = FSlateApplication::Get().GetGameViewport();
	FWidgetPath WidgetUnderMouse = FSlateApplication::Get().LocateWindowUnderMouse(FSlateApplication::Get().GetCursorPos() , FSlateApplication::Get().GetInteractiveTopLevelWindows(), true);
	
	bool bMouseUnderVieport = WidgetUnderMouse.IsValid() &&  WidgetUnderMouse.GetLastWidget() == ViewPort.ToSharedRef();
	
	if (!bMouseUnderVieport)
	{
		return true;
	}
	
	return false;
}

UPlayerHUD* AQuarterViewPlayerController::GetPlayerHUD() const
{
	return PlayerHUD;
}
