// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UntilDawn : ModuleRules
{
	public UntilDawn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "UntilDawn" });

        PublicDependencyModuleNames.AddRange(new string[] 
		{	"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput", 
			"GameplayTasks", 
			"UMG", 
			"AnimGraphRuntime", 
			"UMG", 
			"SlateCore", 
			"Json",
            "GameplayAbilities",
            "GameplayTasks",
            "GameplayTags"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { 
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
