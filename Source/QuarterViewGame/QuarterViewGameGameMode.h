// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuarterViewGameGameMode.generated.h"

UCLASS(minimalapi)
class AQuarterViewGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AQuarterViewGameGameMode();

	//레벨 이동 시 PlayerStart 위치로, 초기 위치 재설정을 위한 메서드 오버라이드
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
protected:
	virtual void BeginPlay() override;
};



