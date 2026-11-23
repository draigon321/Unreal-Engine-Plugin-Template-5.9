# UE 5.9 wizard validation

## v0.1 baseline

- [x] Maintainer observed the Runtime + Editor Framework template in the New Plugin wizard.
- [x] Generated `TestFramework1`, including renamed runtime and editor modules.
- [x] Maintainer built `UE59PluginDevEditor` (Development Editor, Win64); generated DLLs linked successfully.
- [x] Maintainer confirmed both plugins enabled after reopening the editor.
- [ ] Independently rebuild the wizard from a clean source checkout.
- [ ] Package the generated plugin for Win64 and confirm the runtime build does not compile the editor module for game targets.
- [ ] Package and launch a minimal host game with the generated plugin enabled.
- [ ] Verify a packaged wizard still contains all template source files and can generate a new plugin.

## Reproducible packaging procedure (Windows)

Close the editor before running the following from PowerShell. Adjust paths to the installed engine and generated plugin. Use a new output directory; never point `-Package` at the source plugin.

```powershell
& 'C:\Program Files\Epic Games\UE_5.9\Engine\Build\BatchFiles\RunUAT.bat' BuildPlugin `
  '-Plugin=C:\Path\To\Project\Plugins\TestFramework1\TestFramework1.uplugin' `
  '-Package=C:\Path\To\Output\TestFramework1' -TargetPlatforms=Win64
```

Check the automation log for a successful exit and runtime game-target compilation. Editor binaries in a distributable plugin are expected; they must not be linked into a packaged game. Package a minimal C++ host project for Win64 Development, launch it, and inspect its log for runtime module startup and absence of editor-module load errors. Record engine version, host target, command, result and logs. Repeat with a distinct plugin name for future wizard releases.
