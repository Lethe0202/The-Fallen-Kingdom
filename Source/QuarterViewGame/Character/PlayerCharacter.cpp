// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "QuarterViewGameInstance.h"
#include "UIManager.h"
#include "../Manager/QuestManager.h"
#include "../Ability/PlayerAbilityComponent.h"
#include "../Inventory/InventoryComponent.h"
#include "../Quest/QuestLogComponent.h"
#include "Component/ActionComponent.h"
#include "../Interface/InteractableInterface.h"
#include "Blueprint/UserWidget.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "QuarterViewGame/Inventory/ItemQuickSlotComponent.h"

APlayerCharacter::APlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("QuarterViewCamera"));
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
	
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AbilityComponent = CreateDefaultSubobject<UPlayerAbilityComponent>(TEXT("AbilityComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	ItemQuickSlotComponent = CreateDefaultSubobject<UItemQuickSlotComponent>(TEXT("ItemQuickSlotComponent"));
	
	//퀵슬롯에 아이템 Add요청, index, 아이템ID 두종류
	ItemQuickSlotComponent->OnAddItemQuickSlot.AddDynamic(InventoryComponent, &UInventoryComponent::HandleInventoryActionToIndex);
	ItemQuickSlotComponent->OnAddItemQuickSlotToID.AddDynamic(InventoryComponent, &UInventoryComponent::HandleInventoryActionToID);
	
	//인벤토리 액션에 대한 델리게이트
	InventoryComponent->OnInventoryItemAction.AddDynamic(ItemQuickSlotComponent, &UItemQuickSlotComponent::HandleAddItemQuickSlot);
	
	//퀵슬롯에서 아이템 Use요청
	ItemQuickSlotComponent->OnUseItemQuickSlot.AddDynamic(InventoryComponent, &UInventoryComponent::HandleUseItemToID);
	
	//인벤토리에서 정보를 UItemQuickSlotComponent::HandleAddItemQuickSlot로 보냄
	InventoryComponent->OnUpdateInventoryItem.AddDynamic(ItemQuickSlotComponent, &UItemQuickSlotComponent::HandleUpdateItemQuickSlot);
	InventoryComponent->OnUpdateItemCoolingTime.AddDynamic(ItemQuickSlotComponent, &UItemQuickSlotComponent::HandleUpdateItemCoolingTime);
	
	QuestLogComponent = CreateDefaultSubobject<UQuestLogComponent>(TEXT("QuestLogComponent"));
	QuestLogComponent->OnSuccessQuest.AddDynamic(this, &APlayerCharacter::OnSuccessQuest);
	
	InteractionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionCollisionComponent"));
	InteractionCollision->SetupAttachment(GetMesh());
	
	InteractionCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnInteractCollisionOverlap);
	InteractionCollision->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnInteractCollisionEndOverlap);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void APlayerCharacter::Interaction()
{
	if (InteractableActor != nullptr)
	{
		IInteractableInterface* Interactable = Cast<IInteractableInterface>(InteractableActor);
		if (Interactable)
		{
			Interactable->Interact(this);
		}
		else
		{
			// for Blueprint implementation
			IInteractableInterface::Execute_Interact(InteractableActor, this);
		}
	}
}

void APlayerCharacter::InputMove()
{
	if (ActionComponent->CanMove())
	{
		if (GetMesh()->GetAnimInstance()->GetCurrentActiveMontage() != nullptr)
		{
			GetMesh()->GetAnimInstance()->StopAllMontages(0.5f);
		}
	}
}

void APlayerCharacter::DeadCharacter(AActor* DamageCauser)
{
	Super::DeadCharacter(DamageCauser);

	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetGameInstance()))
	{
		if (UUIManager* UIManager = Cast<UUIManager>(QuarterViewGameInstance->GetUIManager()))
		{
			UIManager->ViewDeadScreen();
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerCharacter::OnSuccessQuest(const FName& QuestID)
{
	if (UQuarterViewGameInstance* QuarterViewGameInstance= Cast<UQuarterViewGameInstance>(GetGameInstance()))
	{
		const TArray<FQuestReward> QuestRewardArray = QuarterViewGameInstance->GetQuestManager()->GetRewardItemID(QuestID);

		for (const FQuestReward& QuestReward : QuestRewardArray)
		{
			if (QuestReward.QuestRewardType == EQuestRewardType::Item)
			{
				for (int i = 0; i < QuestReward.Quantity; ++i)
				{
					InventoryComponent->AddItem(QuestReward.ItemID.ToString());
				}
			}
		}
	}
}

void APlayerCharacter::OnInteractCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor->Implements<UInteractableInterface>())
	{
		InteractableActor = OtherActor;
	}
}

void APlayerCharacter::OnInteractCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == InteractableActor)
	{
		InteractableActor = nullptr;
	}
}