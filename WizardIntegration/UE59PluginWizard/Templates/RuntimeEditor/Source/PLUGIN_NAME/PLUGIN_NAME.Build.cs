using UnrealBuildTool;

public class PLUGIN_NAME : ModuleRules
{
    public PLUGIN_NAME(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });
    }
}
