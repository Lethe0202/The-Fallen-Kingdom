// Copyright Epic Games, Inc. All Rights Reserved.


#include "QuestTracker.h"
#if WITH_EDITOR
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "ToolMenus.h"
#include "Framework/Application/SlateApplication.h"
#endif

static const FName QuestTrackerTabName("QuestTracker Utility Widget");

#define LOCTEXT_NAMESPACE "FQuestTrackerModule"

void FQuestTrackerModule::StartupModule()
{
#if WITH_EDITOR
	RegisterQuestTrackerTab();
#endif
}

void FQuestTrackerModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(QuestTrackerTabName);
	UToolMenus::UnregisterOwner(this);
}

void FQuestTrackerModule::RegisterQuestTrackerTab()
{
	UToolMenu* SelectionMainMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");
	
	//MainMenu에 커스텀 메뉴추가
	SelectionMainMenu->AddSubMenu(
		"MainMenu",								//owner
		NAME_None,
		"QuestTracker",								//tool menu name
		INVTEXT("QuestTracker"),					//label
		INVTEXT("My Custom QuestTracker")			//tooltip
	);
	
	UToolMenu* Menu = UToolMenus::Get()->RegisterMenu("LevelEditor.MainMenu.QuestTracker");

	//LevelEditor.MainMenu.QuestTracker메뉴에 섹션추가
	FToolMenuSection& SelectionSectionMain = Menu->AddSection(
		"QuestTracker Section",				//name
		INVTEXT("QuestTracker Section")		//label
	);
	
	//섹션에 실제 클릭 시 동작하도록 기능 추가
	SelectionSectionMain.AddEntry(FToolMenuEntry::InitMenuEntry(
		QuestTrackerTabName,					//name
		INVTEXT("QuestTracker Utility Widget"),			//label
		INVTEXT("Open QuestTracker Utility Widget"),	//tooltip
		FSlateIcon(),
		
		FExecuteAction::CreateLambda([]()
		{
			FGlobalTabmanager::Get()->TryInvokeTab(FName("QuestTracker Utility Widget"));
		})
	));
	
	if (!FGlobalTabmanager::Get()->HasTabSpawner("QuestTracker Utility Widget"))
	{
		//Hidden으로 해야지 다른메뉴(창, 툴)에서 안보임
		FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("QuestTracker Utility Widget"), FOnSpawnTab::CreateRaw(this, &FQuestTrackerModule::OnSpawnTab)).SetMenuType(ETabSpawnerMenuType::Hidden);
	}
}

TSharedRef<SDockTab> FQuestTrackerModule::OnSpawnTab(const FSpawnTabArgs& SpawnTabArgs)
{
	const FString WidgetPath = TEXT("/QuestTracker/EUW_QuestTracker.EUW_QuestTracker");
	UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *WidgetPath);
	
	UEditorUtilitySubsystem* EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();

	//X
	//TSubclassOf<UEditorUtilityWidget> EditorUtilityWidgetClass =  StaticCast<TSubclassOf<UEditorUtilityWidget>>(WidgetBP->GeneratedClass);
	//UEditorUtilityWidget* EditorUtilityWidget = NewObject<UEditorUtilityWidget>(GEditor->GetEditorWorldContext().World(), EditorUtilityWidgetClass);
	//EditorUtilityWidget->AddToRoot();
	
	FName TabID;
	UEditorUtilityWidget* WidgetInstance = EditorUtilitySubsystem->SpawnAndRegisterTabAndGetID(WidgetBP, TabID);
	
	TSharedRef<SDockTab> NewTab = SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		WidgetInstance->TakeWidget()

		//X
		//EditorUtilityWidget->TakeWidget()
	];
	
	//SpawnAndRegisterTabAndGetID에서 하나 NewTab하나 총2개스폰된다. 하지만 실제 사용할 탭은 NewTab이다 때문에 SpawnAndRegisterTabAndGetID으로 생성된 WidgetTab은 닫아주었다.
	EditorUtilitySubsystem->CloseTabByID(TabID);

	//TabName은 엔진의 위젯리플렉터로 찾아서 적용
	TSharedPtr<SDockTab> ExistingTab = FGlobalTabmanager::Get()->FindExistingLiveTab(FName("EUW_QuestTracker_C"));
	if (ExistingTab)
	{
		ExistingTab->RequestCloseTab();	
	}
	
	return NewTab;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuestTrackerModule, QuestTracker)