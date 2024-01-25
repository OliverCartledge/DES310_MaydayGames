// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DES310_MaydayGames : ModuleRules
{
	public DES310_MaydayGames(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput" });
	}
}
