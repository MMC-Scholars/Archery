// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Archery : ModuleRules
{
	public Archery(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Slate", "SlateCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay", "SteamVR" });

        //These allow us to reference files in the VRBase directory from anywhere in the project - you may need to add
        //another path for your machine.
        PublicIncludePaths.Add("C:/Users/Public/Public Projects/Archery/Source/Archery/VRBase");
        PublicIncludePaths.Add("C:/Users/Public/Documents/Public Projects/Archery/Source/Archery/VRBase");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
