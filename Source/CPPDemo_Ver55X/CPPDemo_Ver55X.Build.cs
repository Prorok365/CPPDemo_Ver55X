// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CPPDemo_Ver55X : ModuleRules
{
	public CPPDemo_Ver55X(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
