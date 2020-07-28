// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HorrorPrototype : ModuleRules
{
	public HorrorPrototype(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "AIModule", "NavigationSystem", "Slate", "SlateCore", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] { "FMODStudio" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
