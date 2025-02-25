// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class QuarterViewGame : ModuleRules
{
	public QuarterViewGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
	        "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "Slate", "SlateCore", "LevelSequence", "MovieScene",
	        "AnimationBlueprintLibrary"
        });
        
        if (Target.Type == TargetType.Editor)//Target.bBuildEditor || (Target.Configuration != UnrealTargetConfiguration.Development && Target.Configuration != UnrealTargetConfiguration.Shipping))
        {
	        //PrivateDependencyModuleNames.Add("QuestTracker");
        }
		
        PrivateDependencyModuleNames.AddRange(
	        new string[]
	        {
		        "UMG",
		        // ... add private dependencies that you statically link with here ...	
	        }
        );
        
        PrivateIncludePaths.AddRange(new string[]
        {
            "QuarterViewGame/Ability",
            "QuarterViewGame/AI",
            "QuarterViewGame/Animation",
            "QuarterViewGame/Character",
            "QuarterViewGame/Common",
            "QuarterViewGame/Controller",
            "QuarterViewGame/Inventory",
            "QuarterViewGame/Manager",
            "QuarterViewGame/Quest",
            "QuarterViewGame/UI",
        }
        );
    }
}
