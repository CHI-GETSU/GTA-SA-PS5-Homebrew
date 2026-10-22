# Corresponding source and reconstruction materials — PPSA99604

This directory records the actual inputs of the frozen PS5-tested ZIP, SHA-256
`4aa16e1a012d07a92f29d4607e6d13d5a440b40fba4193b8afc9bfc6575206ee`.
No tested executable or library was changed or rebuilt during this work.

## Source access arrangement

The binary ZIP must be accompanied on the same GitHub Release by
`GTA-SA-PS5-PPSA99604-CORRESPONDING-SOURCE.tar.gz`, its checksum and these
instructions. The source companion contains actual preferred sources, source
archives, local changes, generators, generated inputs, build/conversion recipes
and notices. It is not just a list of upstream URLs or an unfulfilled source
offer. Do not publish the binary by itself. Existing upstream per-file licenses
and authorship remain applicable.

For applicable LGPL combinations the arrangement supplies the library source and
the source of the work using it, together with build/link/conversion materials
(LGPL 2.1 sections 6(a)/(d), and the applicable LGPL 3/GPL 3 conditions). A separate
proprietary object-only relinking kit is not the chosen arrangement. Modifying
libraries and rebuilding/relinking is permitted under the supplied licenses;
there is no project-imposed prohibition on reverse engineering for debugging
those modifications. The public PS5 container conversion/signing tools are
included as source. No private project signing key is required by their recipe.
The project does not supply a PS5 exploit or vendor firmware.

## OpenGL / PSBC / Mesa

`OPENGL_PREFERRED_INPUTS.json` maps all **1,275 object members of 19 installed
archives** to the original object hashes and preferred compile inputs. Installed
materialized archives were compared to original normal/thin archives by object
bytes, not by expecting identical archive headers. The six runtime objects and
platform shim are included. The textual `libPS5OpenGL.a` GROUP is not mistaken
for another compiled library.

The companion contains Mesa 26.2.0 and its 23-file actual source overlay, seven
pinned dependency source exports (including the already patched PSBC tree),
34 generated Mesa/runtime compile sources, 28 PSBC generated source/header
inputs, their preferred generators, and recorded compile/runtime settings.
`OPENGL_DEPENDENCY_FILES.json` identifies the complete exported dependencies;
`PSBC_GENERATED_INPUTS.json` identifies the additional generated inputs.

To prepare a **new** build tree without compiling:

```sh
python3 publication/license-closure/prepare-opengl.py /path/to/companion /path/to/new-opengl-work
```

The preparation checks dependency file hashes and recreates PSBC's source index;
its tree must equal the original `ec0b8c0a52354ca171b3698ced494f2e099cc212`.
In the newly prepared copy only, the host recipe's three Git-HEAD checks are
replaced after verification of the exported source contents. This permits source
archives without distributing private Git metadata. No compiler semantics are
changed. Do not apply the PSBC patch again to the already patched export.

For a later rebuild, use the prepared project's `BUILD_NOBARA.md`,
`build-opengl-local.sh`, and pinned public SDK v0.42. The recorded original
settings are in `original-build-settings/opengl/`; `build.ninja.in` and
`app.ninja.in` are provenance templates with generic root placeholders, not
commands to execute before substituting the chosen workspace paths. The original
Mesa/PSBC source generators and installation Makefiles remain in `ps5-opengl`.
`generated/` in the companion preserves the actual build inputs for comparison;
it does not replace their preferred generators.

## SDK and PacBrew

`STATIC_PACKAGE_MATCHES.json` confirms that all six used PacBrew archives equal
members of the pinned v0.40.2 package, and all three C++/unwind archives equal
members of the official SDK v0.42 ZIP. Unused Ninja prerequisites are not called
linked dependencies. The pinned source commits are:

- SDK: `4eb701204fc3f8d31e84cf8ca272974e2be9c867`.
- PacBrew v0.40.2: `1687e827675c66e1b040751c7f0811222bd3aea0`.

The five original PKGBUILDs are in `pacbrew/` and the complete port repository
snapshot is in the companion. Source archives for curl 8.18.0, libpsl 0.21.5,
OpenSSL 3.5.2, zlib 1.3.2 and zstd 1.5.6 match their recipe SHA-256 values. Curl's
xattr/CA-fetch script edits and autoreconf step are retained; no nonexistent port
patch is invented. The recipes require the pinned SDK's `prospero.sh` environment
and its cross-prefix fix tool. Review PKGBUILDs before executing them in a new
build environment; they are supplied as original build recipes, not run here.
For zstd's library the BSD license option is retained alongside the upstream
GPL text; its separate command-line tools are not inferred to be linked.

`SDK-libcxx.upstream.sh` in `../final-review/` and the SDK source archive specify
LLVM **18.1.8**, its static C++/ABI/unwind configuration, CRT and platform sources.
The full LLVM source archive is supplied. `COMPILER_RT_INPUTS.json` separately
matches all **165 actual compiler-rt builtins objects** to their preferred source
files and `sahin/launcher/tools/build-builtins.sh`. The compiler-rt 18.1.8 source
archive is also included. LLVM, SDK, PacBrew and dependency license/copyright
texts are retained in the companion and existing project notices.

## Wine / TLS reconstruction and relinking

The single clean Wine upstream archive is revision
`490f6d5dcbb2a5047345b8af88d114bbcaad69a8`. Apply exactly one complete delta from
`../final-review/`, then its matching `wine-overlays/` files. Those overlays supply
**32 PS5, 32 host and 33 R4 files** that were untracked during compilation,
including staged Vulkan sources, generated codecs/shared ABI headers and the R4
joystick source. Applying only the tracked-file delta was insufficient.
`WINE_RECONSTRUCTION.json` verifies **37,495 preferred source files**, with no
mismatches against the three actual compile trees.

`restore-wine.py` restores and validates a new source tree without compiling:

```sh
python3 publication/license-closure/restore-wine.py ps5 /path/to/companion/upstream/wine-490f6d5d.tar /path/to/new-source
```

Use `host` or `r4-controller` for the other variants. The archive is hash checked;
existing destinations are refused. `build-ps5-exact.sh`, `build-host-exact.sh` and
`build-r4-exact.sh` are alternate recipes using these complete sources instead of
resetting them to the older 88-patch staging route. They are syntax checked,
not executed during publication. They require the Nobara dependencies documented
in the project's build guides and **new output directories**. Set
`PROSPERO_SOURCE_ROOT` to the published `prospero-win` directory and
`CORRESPONDING_SOURCE_ROOT` to the extracted companion. Supply `--work` and the
pinned SDK/foundation/host-tools paths explicitly for the PS5 and host recipes;
the R4 recipe requires a new `R4_WORK` directory. The host variant is the full
patched Wine build; R4's recipe emits controller targets only. Clean upstream PE
modules use the separately supplied standard `tools/build_wine_runtime.sh`.
Consult `WINE_PE_OUTPUT_MATCHES.json` for which of those outputs is actually used.

GnuTLS **3.8.13** and Nettle **3.10.2** source archives match the actual TLS recipe;
their included libtasn1, libunistring, inih and mini-gmp sources/notices remain
inside those complete archives. `prospero-win/tools/build_tls_ps5.sh` records
all configure options, archive composition and root certificate pin. No external
GMP or hypothetical shared-TLS route is assumed.

`generated-prx/` supplies actual PRX descriptor C sources, including the GnuTLS
export list. `WINE_PLATFORM_INPUTS.json` matches `emutls.o` to the exact SDK
`libc.a`, which itself matches the SDK v0.42 package; SDK `libc/emutls.c` and its
Makefile provide the preferred source and compile recipe. The original libc
wrapper sources, `prx_eh_frame.ld`, module imports and conversion flags are in
`prospero-win/wine/ps5`, `build-ps5-exact.sh` and the preserved native foundations.
The GnuTLS linkage is descriptor + `pw_gnutls_libc` + SDK emutls + GnuTLS/Hogweed/
Nettle static archives + SDK unwind/imports, then the public native-tool PRX
link/sign steps. Library source changes can therefore be incorporated through
this source rebuild/link route. No requirement of byte-identical reproduction
or new PS5 testing is invented as a licensing condition.

Matching native and PRX foundation sources, RADV and Zink Mesa revisions,
PS5_Vulkan and the RADV SDK fork are also included. Their pins and source hashes
are recorded in `ADDITIONAL_SOURCE_SNAPSHOTS.json`; the local mkstemp shim and
portable RADV link recipe remain in `runtime-adaptations/radv-mkstemp`.

## Verification boundaries

Restoring preferred source trees and checking object/source/package hashes does
not claim a fresh compiler run, byte-identical clean builds, or another PS5 test.
The frozen ZIP retains its test-era documents; the Release's current notes and
source companion provide the distribution/source-access instructions. Complete
source and required notices must stay accessible alongside the binary asset.
