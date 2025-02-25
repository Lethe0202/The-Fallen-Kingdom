// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_EDITOR
#include "QuestTrackerItemEntry.h"
#include "QuestTrackerData.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "LevelEditorSubsystem.h"
#endif

void UQuestTrackerItemEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (Cast<UQuestTrackerData>(ListItemObject))
	{
		QuestTrackerData = Cast<UQuestTrackerData>(ListItemObject);
		
		QuestID_Text->SetText(FText::FromName(QuestTrackerData->QuestID));
		QuestDisplayName_Text->SetText(FText::FromName(QuestTrackerData->QuestDisplayName));

		//퀘스트를 사용중인 Actor가 2개이상
		if (QuestTrackerData->LevelName.Num() > 1)
		{
			LevelName_Text->SetText(FText::FromString("Used in 2 or more"));
			ActorName_Text->SetText(FText::FromString("Used in 2 or more"));
			LevelName_Text->SetColorAndOpacity(FColor::Blue);
			TrackingButton->SetColorAndOpacity(FColor::Blue);
			TrackingButton->SetVisibility(ESlateVisibility::Visible);
		}
		//하나의 Actor만 사용중일때
		else if (!QuestTrackerData->LevelName.IsEmpty())
		{
			LevelName_Text->SetText(FText::FromString(QuestTrackerData->LevelName[0]));
			ActorName_Text->SetText(FText::FromString(QuestTrackerData->ActorName[0]));
		}
		//퀘스트를 사용중이지 않을때
		else
		{
			LevelName_Text->SetText(FText::FromString("Not used"));
			ActorName_Text->SetText(FText::FromString("Not used"));

			LevelName_Text->SetColorAndOpacity(FColor::Red);
			TrackingButton->SetColorAndOpacity(FColor::Red);
			TrackingButton->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		
		TrackingButton->OnClicked.AddUniqueDynamic(this, &UQuestTrackerItemEntry::OnTrackingButtonClicked);
	}
	
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
}

void UQuestTrackerItemEntry::NativeOnEntryReleased()
{
	QuestID_Text->SetText(FText::FromString("None"));
	QuestDisplayName_Text->SetText(FText::FromString("None"));
	LevelName_Text->SetText(FText::FromString("None"));
	ActorName_Text->SetText(FText::FromString("None"));
	
	LevelName_Text->SetColorAndOpacity(FColor::White);
	TrackingButton->SetColorAndOpacity(FColor::White);
	
	IUserObjectListEntry::NativeOnEntryReleased();
}

void UQuestTrackerItemEntry::OnTrackingButtonClicked()
{
#if WITH_EDITOR
	if (QuestTrackerData != nullptr)
	{
		if (QuestTrackerData->LevelName.Num() > 1)
		{
			QuestTrackerData->OnShowQuestTracker.Broadcast(QuestTrackerData);
		}
		else
		{
			QuestTrackerData->OnShowQuestTracker.Broadcast(nullptr);
			
			ULevelEditorSubsystem* Subsystem = GEditor->GetEditorSubsystem<ULevelEditorSubsystem>();
			Subsystem->LoadLevel(QuestTrackerData->LevelPath[0]);
			
			if (GEditor && !QuestTrackerData->QuestOwnerActorWorldLocation[0].IsZero())
			{
				for (FEditorViewportClient* ViewportClient : GEditor->GetAllViewportClients())
				{
					if (ViewportClient && ViewportClient->IsPerspective())  // 퍼스펙티브 뷰포트만 제어
					{
						// 카메라 위치 및 회전 설정
						ViewportClient->SetViewLocation(QuestTrackerData->QuestOwnerActorWorldLocation[0]);

						// 뷰포트 갱신
						ViewportClient->Invalidate();
					}
				}
			
				// 액터 선택
				//GEditor->SelectNone(true, true, false);
				//GEditor->SelectActor(QuestTrackerData->QuestOwnertActor.Get(), true, true, true);
			
				// 카메라 이동
				//GEditor->MoveViewportCamerasToActor(*QuestTrackerData->QuestOwnertActor.Get(), false);
			
			}
		}

	}
#endif
}
