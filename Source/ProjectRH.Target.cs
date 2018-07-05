// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectRHTarget : TargetRules
{
	public ProjectRHTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("ProjectRH");

        //Non-Unity-mode
        bUseUnityBuild = false;

        //New standard
        bUsePCHFiles = false;
    }
}
