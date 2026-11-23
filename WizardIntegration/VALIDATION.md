# UE 5.9 wizard validation

## v0.1 baseline

- [x] Maintainer observed the Runtime + Editor Framework template in the New Plugin wizard.
- [x] Generated `TestFramework1`, including renamed runtime and editor modules.
- [x] Maintainer built `UE59PluginDevEditor` (Development Editor, Win64); generated DLLs linked successfully.
- [x] Maintainer confirmed both plugins enabled after reopening the editor.
- [x] Independently rebuild the wizard from a clean source checkout.
- [x] Package the generated plugin for Win64 and confirm the runtime build does not compile the editor module for game targets.
- [x] Package and launch a minimal host game with the generated plugin enabled.
- [x] Verify a packaged wizard contains all four template source files.
- [ ] Generate a new plugin using the packaged wizard in a disposable editor project.

### Packaging evidence (2026-11-22)

Local UE 5.9 AutomationTool `BuildPlugin` for the existing `TestFramework1` completed successfully (exit code 0). With Visual Studio toolchain 14.50.35740 and Windows SDK 10.0.26100.0:

- `UnrealEditor Win64 Development`: both runtime and editor modules compiled and linked.
- `UnrealGame Win64 Development` and `UnrealGame Win64 Shipping`: only the runtime module compiled.
- Clean checkout of v0.1 commit `48fc453`: wizard `BuildPlugin` compiled and linked `UnrealEditor-UE59PluginWizard.dll` and exited 0. All four template files were retained in the package.
- The distributable plugin build is separate from the cooked host-game launch below. The packaged-wizard generation smoke test remains open.

### Cooked host game (2026-11-22)

An isolated copy of the maintainer's UE59PluginDev project, with `TestFramework1` through `TestFramework5` and the v0.2 wizard enabled, successfully built, cooked, staged and archived using UE 5.9.0 (build 60738873), Win64 Development. `BuildCookRun` exited 0. The test map was `/Engine/Maps/Entry`; the copied project included the maintainer's Blueprint and Data Asset test assets.

The actual packaged executable was launched with a test-only `-FrameworkSmokeTest` flag, `-nullrhi`, `-nosound` and `-unattended`. The isolated host's game module checked the loaded modules and cooked assets after map load, logged 17 passing checks, emitted `FRAMEWORK_SMOKE_RESULT=PASS`, and exited 0. `TestFramework1` runtime was loaded and its editor module was absent; the other four framework runtime modules were also loaded and all their editor modules plus the wizard module were absent. Two nonfatal EnhancedInput null-player-input errors were logged in the headless session. This validates runtime loading and cooked assets, not visual rendering or input behavior.

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
- [x] Maintainer confirmed the enabled v0.2 plugin exposes **Tools > Configurable Plugin Wizard** in UE 5.9.
- [x] Maintainer generated and enabled `TestFramework2` with all four options on Nov 22, 2026. Independently inspected its descriptor (Runtime + Editor), all four renamed public headers, function library implementation, UHT generated headers and `UnrealEditor.uclasses` (all four reflected classes). Both DLLs exist and the editor log records loading both modules after restart. No `PLUGIN_NAME` placeholders remain in source.
- [ ] In a disposable C++ project, enable the packaged wizard and restart; confirm the standard tile and separate configurable wizard are both available.
- [x] Maintainer generated and enabled `TestFramework3` with only Actor Component selected. Verified runtime/editor descriptor and DLLs, exactly one reflected runtime class (`TestFramework3Component`), no other scaffold source files, no remaining name placeholders, and both modules loaded in the editor log.
- [x] Maintainer generated and enabled `TestFramework4` with all options unchecked. Verified exactly four base source/build files, Runtime + Editor descriptor, zero reflected classes, no remaining name placeholders, both DLLs present, and both modules loaded in the editor log.
- [x] Maintainer generated and enabled `TestFramework5` with Interface and Function Library selected. Verified both selected headers and function implementation, exactly two reflected runtime classes, no component or data asset scaffold, Runtime + Editor descriptor, both DLLs and both module loads in the editor log; no name placeholders remain.
- [ ] Individually smoke-test Interface, Data Asset and Function Library selections in the editor (all 16 combinations already covered by static fixture testing; representative mixed selection verified above).
- [ ] Close/reopen the configurable wizard and confirm checkboxes reset.
- [x] Blueprint editor usability: maintainer added the component to `BP_FrameworkTest` and compiled; added the interface and observed `OnFrameworkEvent`; created `DA_FrameworkTest` and edited Display Name; placed the `Is Framework Object Valid` pure function in the Event Graph and compiled successfully. Screenshots confirm editor exposure; execution of that function in gameplay was not tested.
- [ ] Package/launch a host game with generated runtime modules and no editor-module load errors.

Run the engine-independent fixture test from the repository root with PowerShell 7 and a new output directory:

```powershell
./WizardIntegration/Test-ScaffoldGeneration.ps1 -OutputDirectory C:/Temp/UE59ScaffoldFixtures
```

`ScaffoldTest0` is the no-options fixture, `ScaffoldTest15` contains all four options. The test mirrors filename/content and module API macro substitution but does not invoke the editor wizard. Package an all-enabled fixture using the same `BuildPlugin` procedure above, substituting its descriptor path. A fixture package is not a substitute for a real wizard UI smoke test.
