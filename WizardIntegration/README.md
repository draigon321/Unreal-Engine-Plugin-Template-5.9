# UE 5.9 New Plugin wizard integration (experimental)

This optional **editor plugin** adds a **Runtime + Editor Framework** tile to **Edit > Plugins > Add/New Plugin**.

## Install

1. Place the `UE59PluginWizard` folder (the directory containing `UE59PluginWizard.uplugin`) in `YourUE59Project/Plugins/`. Do **not** install the entire repository as a nested plugin.
2. Use a C++-enabled Unreal Engine 5.9 project and a supported native compiler.
3. Generate project files, build the Development Editor target, and launch the editor.
4. Enable **UE 5.9 Plugin Wizard Templates** in Edit > Plugins if it is not already enabled; restart.
5. Choose **Add/New Plugin**, select **Runtime + Editor Framework**, enter a plugin name and create it.

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
