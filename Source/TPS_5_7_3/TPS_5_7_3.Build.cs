// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPS_5_7_3 : ModuleRules
{
	public TPS_5_7_3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"TPS_5_7_3",
			"TPS_5_7_3/Variant_Platforming",
			"TPS_5_7_3/Variant_Platforming/Animation",
			"TPS_5_7_3/Variant_Combat",
			"TPS_5_7_3/Variant_Combat/AI",
			"TPS_5_7_3/Variant_Combat/Animation",
			"TPS_5_7_3/Variant_Combat/Gameplay",
			"TPS_5_7_3/Variant_Combat/Interfaces",
			"TPS_5_7_3/Variant_Combat/UI",
			"TPS_5_7_3/Variant_SideScrolling",
			"TPS_5_7_3/Variant_SideScrolling/AI",
			"TPS_5_7_3/Variant_SideScrolling/Gameplay",
			"TPS_5_7_3/Variant_SideScrolling/Interfaces",
			"TPS_5_7_3/Variant_SideScrolling/UI",
			"TPS_5_7_3/Tests",
			"TPS_5_7_3/Public/Tests",
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
