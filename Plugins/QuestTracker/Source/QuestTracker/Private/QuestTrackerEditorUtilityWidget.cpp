// Fill out your copyright notice in the Description page of Project Settings.

#if WITH_EDITOR
#include "QuestTrackerEditorUtilityWidget.h"
#include "QuarterViewGame/Quest/QuestGiverComponent.h"
#include "QuarterViewGame/Common/QuarterViewGameDataTable.h"
#include "QuestTrackerData.h"
#include "Engine/Level.h"
#include "Engine/World.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "EngineUtils.h"
#include "Components/ComboBoxString.h"
#include "Components/ListView.h"
#include "QuestTrackerAllItemWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#endif

void UQuestTrackerEditorUtilityWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UQuestTrackerEditorUtilityWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshQuestGiverNPC();
}

void UQuestTrackerEditorUtilityWidget::FindQuestGiverNPC()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	// 레벨 에셋 필터링
	FARFilter Filter;
	Filter.ClassPaths.Add(FTopLevelAssetPath(TEXT("/Script/Engine"), TEXT("World")));
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Add(FName("/Game/Game/Level"));  // 레벨이 위치한 폴더 경로
	
	TArray<FAssetData> LevelAssets;
	AssetRegistry.GetAssets(Filter, LevelAssets);
	
	for (const FAssetData& AssetData : LevelAssets)
	{
		FString LevelPath = AssetData.GetSoftObjectPath().ToString();
		UE_LOG(LogTemp, Warning, TEXT("Level Found: %s"), *LevelPath);
		
		UWorld* LoadedWorld = LoadObject<UWorld>(nullptr, *LevelPath);
		if (LoadedWorld)
		{
			for (TActorIterator<AActor> It(LoadedWorld); It; ++It)
			{
				AActor* Actor = *It;
				if (Actor)
				{
					if (UQuestGiverComponent* QuestGiverComponent = Actor->FindComponentByClass<UQuestGiverComponent>())
					{
						UE_LOG(LogTemp, Warning, TEXT("Found NPC with QuestGiverComponent: %s"), *Actor->GetName());
						
						const TArray<FName> AllQuestID = QuestGiverComponent->GetAllQuestIDArray();
						for (auto& QuestID : AllQuestID)
						{
							FQuestTrackerInfo QuestTrackerInfo;
							QuestTrackerInfo.LevelPath = LevelPath;
							QuestTrackerInfo.LevelName = AssetData.GetAsset()->GetName();
							QuestTrackerInfo.ActorName = Actor->GetActorLabel();
							QuestTrackerInfo.QuestOwnerActorWorldLocation = Actor->GetActorLocation();
							
							QuestOwnverMap.Add(QuestID, QuestTrackerInfo);
						}
					}
				}
			}
		}
	}
}

void UQuestTrackerEditorUtilityWidget::RefreshQuestGiverNPC()
{
	ClearQuestTracker();
	InitQuestChapter();
	FindQuestGiverNPC();
	
	ChapterComboBoxString->OnSelectionChanged.Clear();
	ChapterComboBoxString->OnSelectionChanged.AddDynamic(this, &UQuestTrackerEditorUtilityWidget::OnChapterChanged);
}

void UQuestTrackerEditorUtilityWidget::HandleAllItemShowNewPanel(const UQuestTrackerData* QuestTrackerData)
{
	QuestTrackerAllItemScrollBox->ClearChildren();
	
	if (QuestTrackerData != nullptr)
	{
		for (int i = 0; i < QuestTrackerData->LevelPath.Num(); ++i)
		{
			UQuestTrackerAllItemWidget* QuestTrackerAllItemWidget = CreateWidget<UQuestTrackerAllItemWidget>(this, QuestTrackerAllItemWidgetClass);
			QuestTrackerAllItemWidget->LevelName_Text->SetText(FText::FromString(QuestTrackerData->LevelName[i]));
			QuestTrackerAllItemWidget->ActorName_Text->SetText(FText::FromString(QuestTrackerData->ActorName[i]));
			QuestTrackerAllItemWidget->LevelPath = QuestTrackerData->LevelPath[i];
			QuestTrackerAllItemWidget->TargetLocation =  QuestTrackerData->QuestOwnerActorWorldLocation[i];
			QuestTrackerAllItemScrollBox->AddChild(QuestTrackerAllItemWidget);
		}
	}
}

void UQuestTrackerEditorUtilityWidget::InitQuestChapter()
{
	TArray<FName> QuestChapterIDArray = QuestChapterDataTable->GetRowNames();
	
	for (auto& QuestChapterID : QuestChapterIDArray)
	{
		ChapterComboBoxString->AddOption(QuestChapterID.ToString());
		QuestChapterMap.Add(QuestChapterID.ToString(), QuestChapterID);
	}
}

void UQuestTrackerEditorUtilityWidget::ClearQuestTracker()
{
	ChapterComboBoxString->ClearOptions();
	QuestListView->ClearListItems();
	QuestTrackerAllItemScrollBox->ClearChildren();
	QuestOwnverMap.Reset();
}

void UQuestTrackerEditorUtilityWidget::OnChapterChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	QuestListView->ClearListItems();
	QuestTrackerAllItemScrollBox->ClearChildren();
	
	for (int i = 1; i <= 100; ++i)
	{
		FString CurrentIDString = FString::Printf(TEXT("%03d"), i * 10);
		
		FString QuestInfoID = SelectedItem;
		QuestInfoID.Append(".");
		QuestInfoID.Append(CurrentIDString);
		
		FQuestInfoRow* QuestInfoRow = QuestDataTable->FindRow<FQuestInfoRow>(*QuestInfoID, TEXT(""));
		if (!QuestInfoRow)
		{
			return;
		}
		
		TArray<FQuestTrackerInfo> OutTrackerInfoList;
		QuestOwnverMap.MultiFind(*QuestInfoID, OutTrackerInfoList);

		//ListView의 Item UObjet 생성 필요한 데이터를 채워넣음
		UQuestTrackerData* QuestTrackerData = NewObject<UQuestTrackerData>();

		//OnShowQuestTracker는 버튼이 눌릴때 Broadcast됨
		QuestTrackerData->OnShowQuestTracker.AddDynamic(this, &UQuestTrackerEditorUtilityWidget::HandleAllItemShowNewPanel);
		
		QuestTrackerData->QuestID = *QuestInfoID;
		QuestTrackerData->QuestDisplayName = *QuestInfoRow->QuestInfoDisplayName.ToString();
		
		for (int QuestTrackerCnt = 0; QuestTrackerCnt < OutTrackerInfoList.Num(); ++QuestTrackerCnt)
		{
			QuestTrackerData->LevelPath.Add(OutTrackerInfoList[QuestTrackerCnt].LevelPath);
			QuestTrackerData->LevelName.Add(OutTrackerInfoList[QuestTrackerCnt].LevelName);
			QuestTrackerData->ActorName.Add(OutTrackerInfoList[QuestTrackerCnt].ActorName);
			QuestTrackerData->QuestOwnerActorWorldLocation.Add(OutTrackerInfoList[QuestTrackerCnt].QuestOwnerActorWorldLocation);
		}
		
		QuestListView->AddItem(QuestTrackerData);
	}
}
