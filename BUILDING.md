# Building and maintaining the beta

This snapshot preserves the delivered launcher and native GTA changes. It is **not yet a verified, self-contained clean build of every runtime component**. A successful source-only import or host test does not establish binary source closure. See `PUBLICATION_REVIEW.md` before distributing compiled output.

## Nobara Linux prerequisites

Use Git, Bash, Python 3, GNU Make, C/C++ compilers, LLVM/Clang/LLD 18 for PS5 native targets, CMake, Ninja, pkg-config, flex, bison, autoconf/automake/libtool, MinGW-w64 32/64-bit compilers, glslang and SPIR-V Tools. Host preview targets also need the relevant curl, EGL/OpenGL and zlib development packages. Package names depend on the Nobara version; no automatic system install is performed by this repository.

For Mesa/Zink Python tools, use a virtual environment and the pinned `prospero-win/tools/mesa_zink_requirements.txt`: Mako 1.3.10, MarkupSafe 3.0.4, Meson 1.9.1, packaging 25.0 and PyYAML 6.0.3. Asset drawing uses Pillow and DejaVu fonts. Do not commit virtual environments or personal configuration.

## Layout

| Directory | Purpose |
|---|---|
| `sahin/launcher` | Adapted delivered GTA launcher and helper build scripts |
| `runner/native-source` | GTA native engine sources, AUTO2 and title lifecycle changes |
| `prospero-win` | Pinned base, profiles, Wine integration patches and runtime recipes |
| `r4-dinput-sources`, `r4-wine` | Controller bridge changes, settings tools and Wine dinput modules |
| `ps5-opengl` | Launcher graphics SDK adaptation and pinned dependency definitions |
| `third-party` | Vendored helper, boilerplate, logging and DXVK sources/notices |
| `runtime-adaptations` | RADV mkstemp shim and historical linking notes |
| `camera-diagnostic` | Read-only settings inspector and targeted host axis tests |

Upstream dependencies are pinned in the scripts and `DEPENDENCIES.md`. Source snapshots omit `.git`, build caches, compiled application files and the extracted SDK toolchain view. Bootstrap scripts fetch the public SDK separately at its pinned hash. DXVK submodules must be obtained at the upstream commit's gitlink revisions; the source snapshot does not include their full contents.

## Host launcher tests

From the repository root:

```sh
python3 test_launcher.py
```

Tests use synthetic data and build under ignored `host-tests/`. No game or PS5 is required. LeakSanitizer can fail in a traced/sandboxed environment; that environment failure must not be reported as a passing test. The prior preparation's relaunch outside that sandbox passed.

## Prepared PS5 build entry points

```sh
bash sahin/launcher/tools/setup-native-dependencies.sh
bash build-opengl-local.sh
bash finish-opengl-local.sh
bash build-launcher-local.sh
python3 build_runner.py
```

These commands are documented entry points, **not a claim that a fresh Nobara machine currently produces the full shipped application without additional work**. The graphics build, game-free Wine prefix and runtime dependencies must be available at their required pins.

The wrapper scripts derive their root from their own location. Overrides include `PS5_CLANG`, `BUILD_JOBS`, `PS5_PAYLOAD_SDK`, `PS5_NATIVE_APP_TEMPLATE`, `PS5_OPENGL_PREFIX`, `PS5_ASSETS_TOOL` and `TMPDIR`. Add LLVM 18's directory to PATH if your distribution installs it outside the default search path.

`build_runner.py` creates a fresh `build/runner-stage`, overlays the GTA native source onto Prospero Win, and compiles all native objects. It requires the launcher helper already built for PPSA99604. It does not reuse the original developer's native object files. Never target a working installation as an output directory.

The complete Wine, TLS, Mesa/RADV, Zink and DXVK runtime is separate from the native title. Its reconstruction uses `prospero-win/tools`, upstream pinned sources and the R4 overrides. The game-free prefix can be prepared with `make_bare_prefix.py CLEAN_PREFIX OUTPUT.zip`, but a reproducible clean-prefix recipe and full runtime closure still need validation. Do not substitute a personal playing prefix.

`package_homebrew.py --help` accepts explicit inputs and requires a new output directory. It prepares a candidate and does not replace the validated release. Its filename checks are not a substitute for archive/license/privacy auditing.

## Historical material and validation boundaries

Files under `docs/*.historical.txt` and historical RADV recipes describe the original laboratory build. They are not advertised as portable commands ready to run on a clean host. Do not apply Sahin's unrelated historical DXVK 3.1.1 patches to the 2.6.2-prospero2 runtime used here.

See `VALIDATION.md` for host test results. Console observations confirm direct launch, Install/Repair in the previous prefix, save behavior, and fresh-console AUTO2 activation. The community first-launch instructions require selecting Joypad inside GTA. No firmware-wide compatibility or clean-build reproducibility guarantee is made.

For a future path-neutral candidate, use consistent `-ffile-prefix-map`, `-fdebug-prefix-map`, `-fmacro-prefix-map`, pinned tools and normalized build timestamps. GnuTLS's optional system configuration path needs separate treatment. The current validated binaries were not rebuilt or edited for this publication preparation.

## Validated PPSA99604 build record

The owner reports launch, DualSense, AUTO2 and right-stick camera working on a real PS5; GTA must be set to Joypad. The exact tested asset and three binary hashes are in `docs/VALIDATED_PPSA99604.json`. Do not rebuild that reference to prepare publication.

The isolated migration build used Clang 18.1.8 for PS5 targets, Clang 22.1.8 for native host tools on Nobara, Python 3.14.7 and Ninja 1.13.2. OpenGL/Wine/runtime dependencies were reused. The launcher wrapper deliberately skips worker, updater, art and libc rebuilds, so those validated inputs must first be staged at its documented Makefile paths. It does not claim a standalone fresh source clone already contains binary dependencies.

For the native runner, PS5_NATIVE_FOUNDATION can point to the pinned prepared boilerplate, with PW_FOUNDATION_READY=1 only when its required SDK/tool/runtime inputs already exist. The script otherwise lets the upstream bootstrap prepare them. Output remains a new runner stage; existing stages are refused. All paths derive from the source tree or explicit overrides, not a fixed developer home directory.

Recovered complete Wine source variants and the launcher Mesa delta are documented in publication/final-review/README.md. They are not cumulative with the standard base patch series. Review LICENSE_CLOSURE.md before distributing any compiled combination.
