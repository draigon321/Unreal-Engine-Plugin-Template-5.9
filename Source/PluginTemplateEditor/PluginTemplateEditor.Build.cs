using UnrealBuildTool;

public class PluginTemplateEditor : ModuleRules
{
    public PluginTemplateEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "PluginTemplate"
        });

        // Add UnrealEd, Slate, ToolMenus, etc. only when used.
    }
}
