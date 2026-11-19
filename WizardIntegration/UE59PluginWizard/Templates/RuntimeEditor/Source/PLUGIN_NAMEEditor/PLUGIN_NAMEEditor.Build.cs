using UnrealBuildTool;

public class PLUGIN_NAMEEditor : ModuleRules
{
    public PLUGIN_NAMEEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivateDependencyModuleNames.AddRange(new string[] { "Core", "PLUGIN_NAME" });
    }
}
