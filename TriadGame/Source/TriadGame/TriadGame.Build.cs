// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TriadGame : ModuleRules
{
	public TriadGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
