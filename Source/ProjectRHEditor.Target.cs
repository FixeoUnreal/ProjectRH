// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectRHEditorTarget : TargetRules
{
	public ProjectRHEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		ExtraModuleNames.Add("ProjectRH");

        //Non-Unity-mode
        bUseUnityBuild = false;

        //New standard
        bUsePCHFiles = false;
    }
}
