// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectRH : ModuleRules
{
	public ProjectRH(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "UMG",
            "GameplayAbilities",
            "GameplayTags",
            "GameplayTasks",
            "OnlineSubsystem",
            "OnlineSubsystemSteam"
        });
	}
}
