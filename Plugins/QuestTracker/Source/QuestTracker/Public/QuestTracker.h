// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


class FQuestTrackerModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	
#if WITH_EDITOR
	void RegisterQuestTrackerTab();
#endif

	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs);
	//TSharedRef<SDockTab> OnSpawnQuestTrackerTab(const FSpawnTabArgs& SpawnTabArgs);
};
