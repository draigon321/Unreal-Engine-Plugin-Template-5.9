# v0.3 proposal: optional Game Control Center integration

Status: design only. The Game Control Center (GCC) plugin source and its public registration/configuration contract have not been supplied or verified. No generated plugin currently depends on GCC, and this proposal deliberately does not invent API names or a plugin descriptor identifier.

## Intended user experience

- Add an independent, unchecked **Game Control Center integration** option to the configurable wizard only once a supported GCC contract is available.
- Keep every combination of the v0.2 scaffolds usable with GCC absent. The original minimal wizard and standalone starter remain unchanged.
- Explain prerequisites before plugin creation. For a requested integration, validate the actual installed GCC plugin, supported version and required module availability; do not silently generate broken or partially integrated projects.
- Generate a small project-specific bridge/configuration scaffold, not the GCC implementation itself. Use the GCC API to register the generated framework when its runtime module is ready and unregister on shutdown, respecting GCC's ownership and lifecycle rules.

## Contract to establish before implementation

| Required evidence | Why |
| --- | --- |
| GCC repository, exact `.uplugin` identifier, version and module names | Resolve the correct dependency and editor/runtime boundary. |
| Public headers and registration/unregistration API | Avoid guessed calls, circular dependencies, or repeated registrations. |
| Required framework identifier, metadata and configuration schema | Produce a valid, stable identifier and editable defaults without overwriting user settings. |
| Startup/shutdown ownership, error and duplicate handling | Ensure deterministic registration and safe cleanup, including reloads. |
| Supported engine/platform targets and packaging policy | Ensure a generated runtime plugin never links editor-only GCC code. |

## Dependency decision

Prefer an isolated bridge module or plugin with a documented GCC runtime dependency for explicitly opted-in projects; leave the base runtime/editor modules free of GCC includes and link dependencies. Whether the bridge may load when GCC is missing depends on the actual GCC contract and UE module-loading rules. A descriptor's `Optional` flag alone does not make statically linked headers or libraries optional. Do not add it, a runtime load call, or a fake registration stub as a substitute for testing the contract.

Use the existing multi-folder template mechanism to stage any proven bridge before UHT/UBT compilation. Validate generated names, update the descriptor and module dependencies together, and keep registration idempotent. No edit of the GCC plugin itself or global project configuration is needed unless separately authorized and required by the documented API.

## Acceptance gates

- GCC absent, option off: build and package without GCC headers, modules, descriptors or configuration.
- GCC installed, option off: no registration, no generated GCC dependency.
- GCC installed and compatible, option on: generated bridge compiles; registers exactly once; presents correct framework metadata; unregisters safely.
- GCC absent/incompatible, option on: actionable pre-generation validation failure (no partial output).
- Win64 UE 5.9 Editor Development, game Development and Shipping packaging; host-game launch checks, plus restart and duplicate-registration tests.
- Review the exact generated source and descriptor for each scenario; record tested GCC version/commit and engine version.

Implementation is blocked until the maintainer identifies the actual GCC repository or supplies its public API and dependency contract. Until then this remains a standalone design, not an advertised v0.3 feature.
