using UnrealBuildTool;

public class UE59PluginWizard : ModuleRules
{
    public UE59PluginWizard(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "Projects",
            "UnrealEd",
            "PluginBrowser"
        });
    }
}
