# UE 5.9 wizard validation

## v0.1 baseline

- [x] Maintainer observed the Runtime + Editor Framework template in the New Plugin wizard.
- [x] Generated `TestFramework1`, including renamed runtime and editor modules.
- [x] Maintainer built `UE59PluginDevEditor` (Development Editor, Win64); generated DLLs linked successfully.
- [x] Maintainer confirmed both plugins enabled after reopening the editor.
- [x] Independently rebuild the wizard from a clean source checkout.
- [x] Package the generated plugin for Win64 and confirm the runtime build does not compile the editor module for game targets.
- [ ] Package and launch a minimal host game with the generated plugin enabled.
- [x] Verify a packaged wizard contains all four template source files.
- [ ] Generate a new plugin using the packaged wizard in a disposable editor project.

### Packaging evidence (2026-11-22)

Local UE 5.9 AutomationTool `BuildPlugin` for the existing `TestFramework1` completed successfully (exit code 0). With Visual Studio toolchain 14.50.35740 and Windows SDK 10.0.26100.0:

- `UnrealEditor Win64 Development`: both runtime and editor modules compiled and linked.
- `UnrealGame Win64 Development` and `UnrealGame Win64 Shipping`: only the runtime module compiled.
- Clean checkout of v0.1 commit `48fc453`: wizard `BuildPlugin` compiled and linked `UnrealEditor-UE59PluginWizard.dll` and exited 0. All four template files were retained in the package.
- This was a distributable plugin build, not a cooked host game launch. Host-game and packaged-wizard smoke tests remain open.

## Reproducible packaging procedure (Windows)

Close the editor before running the following from PowerShell. Adjust paths to the installed engine and generated plugin. Use a new output directory; never point `-Package` at the source plugin.

```powershell
& 'C:\Program Files\Epic Games\UE_5.9\Engine\Build\BatchFiles\RunUAT.bat' BuildPlugin `
  '-Plugin=C:\Path\To\Project\Plugins\TestFramework1\TestFramework1.uplugin' `
  '-Package=C:\Path\To\Output\TestFramework1' -TargetPlatforms=Win64
```

Check the automation log for a successful exit and runtime game-target compilation. Editor binaries in a distributable plugin are expected; they must not be linked into a packaged game. Package a minimal C++ host project for Win64 Development, launch it, and inspect its log for runtime module startup and absence of editor-module load errors. Record engine version, host target, command, result and logs. Repeat with a distinct plugin name for future wizard releases.

## v0.2 configurable wizard

- [x] UE 5.9 `BuildPlugin` compiled and linked `UE59PluginWizard` and exited 0 on Win64.
- [x] Inspected the resulting package: every base template and scaffold source file is present (9 files total).
- [x] Static fixture generation for all 16 checkbox combinations verified selected/omitted headers, no collisions and placeholder substitution.
- [x] Compile/package the all-enabled generated fixture (including Unreal Header Tool reflection): Nov 22, 2026, `ScaffoldTest15` `BuildPlugin` exited 0; Editor Development linked both modules and game Development/Shipping compiled the reflected runtime classes only.
- [ ] In a disposable C++ project, enable the packaged wizard and restart; confirm the standard tile and separate configurable wizard are both available.
- [ ] In the configurable wizard, generate fresh plugins for none, each single option, a mixed pair, and all options. Verify descriptor and source names and that unchecked scaffolds are absent.
- [ ] Close/reopen the configurable wizard and confirm checkboxes reset. Verify each generated Blueprint class is discoverable and usable in editor.
- [ ] Package/launch a host game with generated runtime modules and no editor-module load errors.

Run the engine-independent fixture test from the repository root with PowerShell 7 and a new output directory:

```powershell
./WizardIntegration/Test-ScaffoldGeneration.ps1 -OutputDirectory C:/Temp/UE59ScaffoldFixtures
```

`ScaffoldTest0` is the no-options fixture, `ScaffoldTest15` contains all four options. The test mirrors filename/content and module API macro substitution but does not invoke the editor wizard. Package an all-enabled fixture using the same `BuildPlugin` procedure above, substituting its descriptor path. A fixture package is not a substitute for a real wizard UI smoke test.
