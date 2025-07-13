// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SteamTest544 : ModuleRules
{
	public SteamTest544(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam" });
	}
}
