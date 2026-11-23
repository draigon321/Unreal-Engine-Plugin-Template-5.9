# UE 5.9 New Plugin wizard integration (experimental)

This optional **editor plugin** adds a **Runtime + Editor Framework** tile to **Edit > Plugins > Add/New Plugin**.

## Install

1. Place the `UE59PluginWizard` folder (the directory containing `UE59PluginWizard.uplugin`) in `YourUE59Project/Plugins/`. Do **not** install the entire repository as a nested plugin.
2. Use a C++-enabled Unreal Engine 5.9 project and a supported native compiler.
3. Generate project files, build the Development Editor target, and launch the editor.
4. Enable **UE 5.9 Plugin Wizard Templates** in Edit > Plugins if it is not already enabled; restart.
5. Choose **Add/New Plugin**, select **Runtime + Editor Framework**, enter a plugin name and create it.

## Configurable wizard (v0.2)

The original New Plugin tile remains available for the minimal runtime/editor starter. For optional scaffolds, open **Window > Configurable Plugin Wizard** (Tools group), select the framework template, and tick any combination of these options before creating the plugin:

| Option | Generated runtime class |
| --- | --- |
| Blueprint Actor Component | `U<PluginName>Component`, Blueprint-spawnable component |
| Blueprint Interface | `U<PluginName>Interface` / `I<PluginName>Interface`, with an implementable event |
| Blueprint Data Asset | `U<PluginName>DataAsset`, Blueprint-visible display name property |
| Blueprint Function Library | `U<PluginName>FunctionLibrary`, example pure validity function |

All options start unchecked each time the configurable wizard opens. No options still creates the two-module framework. These are native C++ classes exposed to Blueprints, not pre-authored `.uasset` files; use the editor to create Blueprint subclasses, add components, implement the interface, and create data asset instances. No GCC dependency is introduced.

Scaffolds are copied by Unreal's PluginUtils before compilation, so the normal wizard build includes reflected classes via Unreal Header Tool. The runtime module exports Core, CoreUObject and Engine as public dependencies for consumers of those headers. The configurable wizard is project-only and requires a C++ project.

See [validation checklist](VALIDATION.md) for regression checks and build evidence.

## Intended output

The wizard should create a renamed runtime module `<YourName>` and editor-only module `<YourName>Editor`. The template registers through `IPluginBrowser::RegisterPluginTemplate`, with `FPluginTemplateDescription` pointing at the included `Templates/RuntimeEditor` folder.

## v0.1 validation status

- In the maintainer's UE 5.9 / Visual Studio Development Editor test, the wizard generated `TestFramework1`; its runtime and editor DLLs compiled and linked successfully. The maintainer subsequently confirmed both the wizard and generated plugin were enabled in the editor.
- The build reported a nonblocking Fab / ToolsetRegistry dependency warning. Close the editor before building from Visual Studio to avoid Live Coding conflicts.
- This verifies the generated two-module baseline, not a clean rebuild of every repository module. See [validation checklist](VALIDATION.md) for packaging and regression checks.
- The `PLUGIN_NAME` placeholder follows the Unreal PluginUtils naming convention; verify its substitution in both file paths and file contents in the specific 5.9 build.
- This is a minimal C++ runtime/editor starter, not a Blueprint-only plugin, project template, or GCC implementation.
- If the wizard does not create both modules properly, report the generated descriptor and build log before using generated plugins in production.
- When packaging plugins, ensure editor-only code is excluded from runtime targets.

## Engine reference

See Epic APIs `Features/IPluginsEditorFeature.h`, `IPluginBrowser.h`, and `PluginUtils.h` for the template registration and generation interfaces.
