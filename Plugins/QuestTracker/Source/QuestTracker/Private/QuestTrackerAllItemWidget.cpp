// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_EDITOR
#include "QuestTrackerAllItemWidget.h"
#include "QuestTrackerData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "LevelEditorSubsystem.h"
#endif

void UQuestTrackerAllItemWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TrackingButton->OnClicked.AddDynamic(this, &UQuestTrackerAllItemWidget::OnTrackerButtonClicked);
}

void UQuestTrackerAllItemWidget::OnTrackerButtonClicked()
{
	ULevelEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
	Subsystem->LoadLevel(LevelPath);
	
	for (FEditorViewportClient* ViewportClient : GEditor->GetAllViewportClients())
	{
		if (ViewportClient && ViewportClient->IsPerspective())  // 퍼스펙티브 뷰포트만 제어
		{
			// 카메라 위치 설정
			ViewportClient->SetViewLocation(TargetLocation);

			// 뷰포트 갱신
			ViewportClient->Invalidate();
		}
	}
}
