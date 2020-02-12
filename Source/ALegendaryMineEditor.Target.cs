// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ALegendaryMineEditorTarget : TargetRules
{
	public ALegendaryMineEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.Add("ALegendaryMine");
        ExtraModuleNames.Add("RoomTemplateDesigner");
    }
}
