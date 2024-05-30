using UnrealBuildTool;
 
public class RecoilSystemEditor : ModuleRules
{
	public RecoilSystemEditor(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "UnrealEd",
                "InputCore", "SlateCore", "Slate", "GraphEditor" });
        PrivateDependencyModuleNames.AddRange(new string[] { "RecoilSystem"});
 
		PublicIncludePaths.AddRange(new string[] {"RecoilSystemEditor/Public"});
		PrivateIncludePaths.AddRange(new string[] {"RecoilSystemEditor/Private"});
	}
}