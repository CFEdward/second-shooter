// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class SecondShooterEditorTarget : TargetRules
{
	public SecondShooterEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		bOverrideBuildEnvironment = true;

        ExtraModuleNames.AddRange( new string[] { "SecondShooter" } );
	}
}
