// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RT_FPS : ModuleRules
{
	public RT_FPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "RT_FPS" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
        "GameplayAbilities", "GameplayTags", "GameplayTasks", "AIModule" });

        PrivateDependencyModuleNames.AddRange(
            new string[] { "NetCore", "RecoilSystem" }
        );
    }
}
