// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "UObject/NoExportTypes.h"
#include "QVEventManager.generated.h"


class ULevelSequence;

UCLASS(Blueprintable)
class QUARTERVIEWGAME_API UQVEventManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "QVEventManager")
	void PlayLevelSequence(ULevelSequence* LevelSequence, FMovieSceneSequencePlaybackSettings Settings);
	
protected:
	UFUNCTION()
	void OnSequenceFinished();
};
