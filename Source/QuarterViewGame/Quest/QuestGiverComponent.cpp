// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiverComponent.h"

#include "SaveManager.h"
#include "../UI/Quest/QuestPanelWidget.h"
#include "../UI/Quest/QuestStateWidget.h"
#include "../Common/QuarterViewGameDataTable.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WidgetComponent.h"

#include "kismet/GameplayStatics.h"

// Sets default values for this component's properties
UQuestGiverComponent::UQuestGiverComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called every frame
void UQuestGiverComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Called when the game starts
void UQuestGiverComponent::BeginPlay()
{
	Super::BeginPlay();
	
	//QuestState Map의 각 상태와 배열 초기화
	TArray<FName> WaitingQuestID;
	TArray<FName> InProgressQuestID;
	TArray<FName> CompletionWaitingQuestID;
	TArray<FName> SuccessQuestID;
	TArray<FName> LockQuestID;
	
	QuestState.Add(EQuestState::Waiting, WaitingQuestID);
	QuestState.Add(EQuestState::InProgress, InProgressQuestID);
	QuestState.Add(EQuestState::CompletionWaiting, CompletionWaitingQuestID);
	QuestState.Add(EQuestState::Success, SuccessQuestID);
	QuestState.Add(EQuestState::Lock, LockQuestID);
	
	//Component Owner에게 QuestStateWidget 생성
	if (QuestStateWidgetClass)
	{
		WidgetComponent = NewObject<UWidgetComponent>(GetOwner());
		if (WidgetComponent)
		{
			WidgetComponent->RegisterComponent();
			WidgetComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
			WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
			WidgetComponent->SetDrawSize(FVector2D(50, 100));
			WidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
			WidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			
			UQuestStateWidget* QuestStateWidget = CreateWidget<UQuestStateWidget>(GetWorld(), QuestStateWidgetClass);
			QuestStateWidget->SetOwnerID(GetOwner()->GetUniqueID());
			WidgetComponent->SetWidget(QuestStateWidget);
		}
	}
	
	InitQuestMap();
}

void UQuestGiverComponent::InitQuestMap()
{
	for (FDataTableRowHandle QuestInfoRowHandle : QuestInfoRowHandleArray)
	{
		FQuestInfoRow* QuestInfoRow = QuestInfoRowHandle.GetRow<FQuestInfoRow>(TEXT(""));

		if (QuestInfoRow != nullptr)
		{
			if (QuestInfoRow->PreQuestArray.IsEmpty())
			{
				QuestState[EQuestState::Waiting].Add(QuestInfoRowHandle.RowName);
			}
			else
			{
				QuestState[EQuestState::Lock].Add(QuestInfoRowHandle.RowName);
				
				//변수 주석 참고
				if (PreQuestForLockedQuestMap.Contains(QuestInfoRowHandle.RowName))
				{
					PreQuestForLockedQuestMap[QuestInfoRowHandle.RowName].Append(QuestInfoRow->PreQuestArray);
				}
				else
				{
					TArray<FName> PreQuestArray;
					PreQuestForLockedQuestMap.Add(QuestInfoRowHandle.RowName, PreQuestArray);
					PreQuestForLockedQuestMap[QuestInfoRowHandle.RowName].Append(QuestInfoRow->PreQuestArray);
				}
				
				for (int i = 0; i < QuestInfoRow->PreQuestArray.Num(); ++i)
				{
					if (PreQuestCache.Contains(QuestInfoRow->PreQuestArray[i]))
					{
						PreQuestCache[QuestInfoRow->PreQuestArray[i]].Add(QuestInfoRowHandle.RowName);	
					}
					else
					{
						TArray<FName> LockQuestArray;
						PreQuestCache.Add(QuestInfoRow->PreQuestArray[i], LockQuestArray);
						PreQuestCache[QuestInfoRow->PreQuestArray[i]].Add(QuestInfoRowHandle.RowName);	
					}
				}
			}
		}
	}
	
	if (WidgetComponent)
	{
		if (UQuestStateWidget* QuestStateWidget = Cast<UQuestStateWidget>(WidgetComponent->GetWidget()))
		{
			if (QuestState[EQuestState::Waiting].IsValidIndex(0))
			{
				QuestStateWidget->ChangeState(EQuestState::Waiting);
			}
			else
			{
				QuestStateWidget->ChangeState(EQuestState::Lock);	
			}
		}
	}
}

void UQuestGiverComponent::QuestInterct(AActor* Interactor)
{
	UQuestLogComponent* QuestLogComponent = Interactor->GetComponentByClass<UQuestLogComponent>();
	if (QuestLogComponent == nullptr)
	{
		return;
	}
	
	//CompletionWaiting 퀘스트 존재하고, 성공햇다면
	if (QuestState[EQuestState::CompletionWaiting].IsValidIndex(0) && QuestLogComponent->IsCompletionQuest(QuestState[EQuestState::CompletionWaiting][0]))
	{
		FName SuccessQuestID = QuestState[EQuestState::CompletionWaiting][0];
		
		QuestState[EQuestState::Success].Add(QuestState[EQuestState::CompletionWaiting][0]);
		QuestState[EQuestState::CompletionWaiting].RemoveAt(0);
		
		//아이템 보상이 있는데 보상이 지급될때, 아이템획득 퀘스트 관련해서 문제가있어서 맨 마지막에서 SuccessQuest호출 
		QuestLogComponent->SuccessQuest(SuccessQuestID);
	}
	else
	{
		if(QuestPanelWidget->IsValidLowLevel())
		{
			QuestPanelWidget->RemoveFromParent();
		}
		
		//진행중인 퀘스트 있으면
		if (QuestState[EQuestState::InProgress].IsValidIndex(0) && QuestLogComponent->IsActivateQuest(QuestState[EQuestState::InProgress][0]))
		{
			QuestPanelWidget = Cast<UQuestPanelWidget>(CreateWidget(GetWorld(), QuestPanelWidgetClass));
			QuestPanelWidget->InitQuestPanel(QuestState[EQuestState::InProgress][0], Interactor, EQuestPanelType::InProgress);
			QuestPanelWidget->AddToViewport(1);
		}
		else if (QuestState[EQuestState::Waiting].IsValidIndex(0))
		{
			QuestPanelWidget = Cast<UQuestPanelWidget>(CreateWidget(GetWorld(), QuestPanelWidgetClass));
			QuestPanelWidget->InitQuestPanel(QuestState[EQuestState::Waiting][0], Interactor, EQuestPanelType::Accept);
			QuestPanelWidget->AddToViewport(1);
		}
	}
}

void UQuestGiverComponent::QuestConditionCompleted(const FName& QuestID)
{
	if (QuestState[EQuestState::InProgress].IsValidIndex(0) && QuestState[EQuestState::InProgress].Contains(QuestID))
	{
		QuestState[EQuestState::InProgress].Remove(QuestID);
		QuestState[EQuestState::CompletionWaiting].Add(QuestID);

		if (UQuestStateWidget* QuestStateWidget = Cast<UQuestStateWidget>(WidgetComponent->GetWidget()))
		{
			QuestStateWidget->ChangeState(EQuestState::CompletionWaiting);
		}
	}
}

void UQuestGiverComponent::UpdateSuccessfulQuest(const FName& QuestID)
{
	for (auto& QuestArray : QuestState)
	{
		for (auto& QuestName : QuestArray.Value)
		{
			if (QuestName == QuestID)
			{
				QuestArray.Value.Remove(QuestID);
				QuestState[EQuestState::Success].Add(QuestID);
			}
		}
	}
	
	if (PreQuestCache.Contains(QuestID))
	{
		for (int i = 0; i < PreQuestCache[QuestID].Num(); ++i)
		{
			FName LockQuestName = PreQuestCache[QuestID][i];
			PreQuestForLockedQuestMap[LockQuestName].Remove(QuestID);
			PreQuestCache[QuestID].Remove(LockQuestName);
			
			if (PreQuestForLockedQuestMap[LockQuestName].IsEmpty())
			{
				//퀘스트 잠금 해제
				PreQuestForLockedQuestMap.Remove(LockQuestName);
				
				if (QuestState[EQuestState::Lock].Contains(LockQuestName))
				{
					QuestState[EQuestState::Lock].Remove(LockQuestName);
					QuestState[EQuestState::Waiting].Add(LockQuestName);

					if (UQuestStateWidget* QuestStateWidget = Cast<UQuestStateWidget>(WidgetComponent->GetWidget()))
					{
						QuestStateWidget->ChangeState(EQuestState::Waiting);	
					}
				}
			}
		}
	}
}

void UQuestGiverComponent::UpdateInProgressQuest(const FName& QuestID)
{
	TArray<FName> WaitingQuestArray = QuestState[EQuestState::Waiting];
	if (WaitingQuestArray.Contains(QuestID))
	{
		QuestState[EQuestState::Waiting].Remove(QuestID);
		QuestState[EQuestState::InProgress].Add(QuestID);
	}
}

void UQuestGiverComponent::UpdateQuestState()
{
	if (!WidgetComponent)
	{
		return;
	}
	
	UQuestStateWidget* QuestStateWidget = Cast<UQuestStateWidget>(WidgetComponent->GetWidget());
	if (!QuestStateWidget)
	{
		return;
	}
	
	if (QuestState[EQuestState::CompletionWaiting].IsValidIndex(0))
	{
		QuestStateWidget->ChangeState(EQuestState::CompletionWaiting);
	}
	else if(QuestState[EQuestState::InProgress].IsValidIndex(0))
	{
		QuestStateWidget->ChangeState(EQuestState::InProgress);
	}
	else if (QuestState[EQuestState::Waiting].IsValidIndex(0))
	{
		QuestStateWidget->ChangeState(EQuestState::Waiting);
	}
	else
	{
		QuestStateWidget->ChangeState(EQuestState::Lock);	
	}
}

void UQuestGiverComponent::OnInteractorAddQuest(const FName& QuestID, const FText& QuestDisplayName)
{
	if (QuestState[EQuestState::Waiting].IsValidIndex(0) && QuestState[EQuestState::Waiting][0] == QuestID)
	{
		QuestState[EQuestState::InProgress].Add(QuestState[EQuestState::Waiting][0]);
		QuestState[EQuestState::Waiting].RemoveAt(0);
	
		if (UQuestStateWidget* QuestStateWidget = Cast<UQuestStateWidget>(WidgetComponent->GetWidget()))
		{
			QuestStateWidget->ChangeState(EQuestState::InProgress);
		}
	}
}

void UQuestGiverComponent::OnInteractorRemoveQuest(const FName& QuestID)
{
	if (QuestState[EQuestState::InProgress].IsValidIndex(0) && QuestState[EQuestState::InProgress][0] == QuestID)
	{
		QuestState[EQuestState::Waiting].Add(QuestState[EQuestState::InProgress][0]);
		QuestState[EQuestState::InProgress].RemoveAt(0);
	}
	
	if (UQuestStateWidget* QuestStateWidget = Cast<UQuestStateWidget>(WidgetComponent->GetWidget()))
	{
		if (QuestState[EQuestState::Waiting].IsValidIndex(0) && QuestState[EQuestState::Waiting].IsValidIndex(0))
		{
			QuestStateWidget->ChangeState(EQuestState::Waiting);
		}
		else
		{
			QuestStateWidget->ChangeState(EQuestState::Lock);
		}
	}
}

const bool UQuestGiverComponent::IsProgressQuest(const FName& QuestID) const
{
	if (QuestState[EQuestState::InProgress].Contains(QuestID))
	{
		return true;
	}

	return false;
}

const TArray<FName> UQuestGiverComponent::GetAllQuestIDArray() const
{
	TArray<FName> QuestIDArray;
	for (auto& QuestInfoRowHandle : QuestInfoRowHandleArray)
	{
		QuestIDArray.Add(QuestInfoRowHandle.RowName);
	}

	return QuestIDArray;
}

TSoftObjectPtr<UTexture2D> UQuestGiverComponent::GetCurrentStateIcon() const
{
	if (WidgetComponent != nullptr)
	{
		return Cast<UQuestStateWidget>(WidgetComponent->GetWidget())->GetTexture();
	}

	return nullptr;
}

FColor UQuestGiverComponent::GetCurrentStateIconColor() const
{
	if (WidgetComponent != nullptr)
	{
		return Cast<UQuestStateWidget>(WidgetComponent->GetWidget())->GetColor().ToFColor(true);
	}

	return FColor::White;
}
