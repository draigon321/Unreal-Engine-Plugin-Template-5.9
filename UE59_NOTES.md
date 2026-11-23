# Unreal Engine 5.9 adaptation notes

This fork is based on [JDSherbert's Unreal Engine Plugin Template](https://github.com/JDSherbert/Unreal-Engine-Plugin-Template), licensed under the Unlicense (see LICENSE).

## Status

The optional [UE 5.9 plugin wizard](WizardIntegration/README.md) generated a runtime/editor plugin that the maintainer compiled and enabled in UE 5.9. The standalone starter and packaging have separate validation requirements; see the [validation checklist](WizardIntegration/VALIDATION.md).

## Quick start

1. Copy `PluginTemplate.uplugin` and `Source/` into `YourProject/Plugins/PluginTemplate`. Install the optional wizard separately using its own instructions.
2. Open a **C++ Unreal Engine 5.9 project** (or add a C++ class to a Blueprint-only project) with a supported native compiler toolchain installed.
3. Regenerate project files and build the **Development Editor** target.
4. Launch the editor and verify that **Plugin Template (UE 5.9)** is enabled under Plugins.
5. For a new plugin, replace every `PluginTemplate` / `PluginTemplateEditor` module name consistently across folder names, `.Build.cs` classes, C++ module registration, and the `.uplugin` descriptor. Rename the descriptor to match the plugin directory name.

## Project templates vs plugin templates

This repository contains a standalone **plugin starter** and an optional **New Plugin wizard** integration. It is not a native **New Project** template.

## Test checklist

- [ ] UE 5.9 Development Editor compilation
- [ ] Both modules load without warnings
- [ ] Plugin works inside a test project `Plugins/` folder
- [ ] Packaged runtime target excludes editor module
- [ ] Renamed copy compiles with new module names
