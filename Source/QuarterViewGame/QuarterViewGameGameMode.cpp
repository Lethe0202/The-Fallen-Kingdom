// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuarterViewGameGameMode.h"
#include "QuarterViewGameInstance.h"
#include "QuestManager.h"
#include "SaveManager.h"
#include "UObject/ConstructorHelpers.h"

AQuarterViewGameGameMode::AQuarterViewGameGameMode()
{
}

void AQuarterViewGameGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	
	//? 제거
	FString StartPointOptionString = OptionsString.Mid(1, OptionsString.Len() - 1);
	
	AActor* StartPoint = FindPlayerStart(NewPlayer, StartPointOptionString);

	if (APawn* PlayerPawn = NewPlayer->GetPawn())
	{
		PlayerPawn->SetActorLocation(StartPoint->GetActorLocation());
		PlayerPawn->SetActorRotation(StartPoint->GetActorRotation());
	}
}

void AQuarterViewGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UQuarterViewGameInstance* QuarterViewGameInstance= Cast<UQuarterViewGameInstance>(GetGameInstance()))
	{
		if (USaveManager* SaveManager = QuarterViewGameInstance->GetSaveManager())
		{
			SaveManager->InitSaveManager(QuarterViewGameInstance->GetLocalPlayerByIndex(0)->GetPlayerController(GetWorld()));
		}

		if (UQuestManager* QuestManager = QuarterViewGameInstance->GetQuestManager())
		{
			QuestManager->InitQuestDelegate();
		}
	}
}
