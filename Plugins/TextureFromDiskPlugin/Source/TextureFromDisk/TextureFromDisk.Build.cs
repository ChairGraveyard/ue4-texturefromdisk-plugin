// Copyright 1998-2013 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TextureFromDisk : ModuleRules
{
	public TextureFromDisk(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore",
				"RHI"
			}
		);
		//Private Paths
        PrivateIncludePaths.AddRange(new string[] { 
			"TextureFromDisk/Public",
			"TextureFromDisk/Private"
		});
	}
}
