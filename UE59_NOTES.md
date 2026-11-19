# Unreal Engine 5.9 adaptation notes

This fork is based on [JDSherbert's Unreal Engine Plugin Template](https://github.com/JDSherbert/Unreal-Engine-Plugin-Template), licensed under the Unlicense (see LICENSE).

## Status

The descriptor and C++ module skeleton have been updated for an **intended Unreal Engine 5.9 target**. **Compilation and editor startup have not yet been verified against an installed UE 5.9 build.**

## Quick start

1. Copy the entire repository into `YourProject/Plugins/PluginTemplate`.
2. Open a **C++ Unreal Engine 5.9 project** (or add a C++ class to a Blueprint-only project) with a supported native compiler toolchain installed.
3. Regenerate project files and build the **Development Editor** target.
4. Launch the editor and verify that **Plugin Template (UE 5.9)** is enabled under Plugins.
5. For a new plugin, replace every `PluginTemplate` / `PluginTemplateEditor` module name consistently across folder names, `.Build.cs` classes, C++ module registration, and the `.uplugin` descriptor. Rename the descriptor to match the plugin directory name.

## Project templates vs plugin templates

This repository is a **plugin starter**, not a native **New Project** template or **New Plugin wizard** template. Those integrations require separate template definitions / editor wizard customization and should be implemented and tested separately.

## Test checklist

- [ ] UE 5.9 Development Editor compilation
- [ ] Both modules load without warnings
- [ ] Plugin works inside a test project `Plugins/` folder
- [ ] Packaged runtime target excludes editor module
- [ ] Renamed copy compiles with new module names
