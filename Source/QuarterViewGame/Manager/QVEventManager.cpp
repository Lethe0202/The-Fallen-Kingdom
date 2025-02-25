// Fill out your copyright notice in the Description page of Project Settings.


#include "QVEventManager.h"
#include "../Manager/QuarterViewGameInstance.h"
#include "../Manager/UIManager.h"

#include "MovieSceneSequencePlaybackSettings.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "LevelSequence.h"
#include "HUD/PlayerHUD.h"

void UQVEventManager::PlayLevelSequence(ULevelSequence* LevelSequence, FMovieSceneSequencePlaybackSettings Settings)
{
	ALevelSequenceActor* SequenceActor;
	ULevelSequencePlayer* LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, Settings, SequenceActor);

	//HUD Hide
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UUIManager* UIManager = QuarterViewGameInstance->GetUIManager())
		{
			UIManager->GetPlayerHUD()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
	//Change InputMode
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->SetInputMode(FInputModeUIOnly());
	}
	
	LevelSequencePlayer->OnFinished.AddDynamic(this, &UQVEventManager::OnSequenceFinished);	

	if (LevelSequencePlayer != nullptr)
	{
		LevelSequencePlayer->Play();
	}
}

void UQVEventManager::OnSequenceFinished()
{
	if (UQuarterViewGameInstance* QuarterViewGameInstance = Cast<UQuarterViewGameInstance>(GetWorld()->GetGameInstance()))
	{
		if (UUIManager* UIManager = QuarterViewGameInstance->GetUIManager())
		{
			UIManager->GetPlayerHUD()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
	}

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeGameAndUI InputModeGameAndUI;
		InputModeGameAndUI.SetHideCursorDuringCapture(false);
		PC->SetInputMode(InputModeGameAndUI);
	}
}
