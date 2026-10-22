# Recovered source and build correspondence

These records are data, not build instructions to execute blindly. Absolute personal build paths were replaced by generic roots; matched file contents were not modified.

- `CORRESPONDENCE_EVIDENCE.json`: PRX hashes, exact TLS source/archive checks, Lapy preferred-source hashes, static link/SDK/Mesa/PSBC evidence. Archive files tracked as Ninja prerequisites are listed separately from direct linker inputs; unused dependencies are not claimed to be linked.
- `WINE_PE_OUTPUT_MATCHES.json`: all 1450 shipped Wine PE DLL/EXE files matched to their actual local output files. Laboratory-relative paths describe evidence, not portable commands.
- `CONTROLLER_BUILD_MATCHES.json`: explains distinct 32-bit R4 versus 64-bit base controller DLL provenance.
- `wine-ps5-from-490f6d5d.patch`, `wine-host-from-490f6d5d.patch`, `wine-r4-controller-from-490f6d5d.patch`: complete source deltas from Wine `490f6d5dcbb2a5047345b8af88d114bbcaad69a8`. Each was reverse-checked against its actual compile tree. Fetch the [upstream Wine revision](https://gitlab.winehq.org/wine/wine/-/tree/490f6d5dcbb2a5047345b8af88d114bbcaad69a8), or the same revision from the [GitHub mirror](https://github.com/wine-mirror/wine/tree/490f6d5dcbb2a5047345b8af88d114bbcaad69a8), and apply exactly the matching complete patch to a fresh tree. Do not also apply the base 88-patch series. Existing standard scripts use that series; these complete deltas are an alternate source-reconstruction record, not an instruction to double-patch their output.
- `mesa-launcher-from-26.2.0.patch`: changes to 23 original members of the SHA-pinned Mesa 26.2.0 source archive. Generation/platform recipes remain in ps5-opengl; additional inputs are an explicit remaining review scope.
- `SDK-libcxx.upstream.sh`: unmodified upstream GPL source recipe from [SDK commit 4eb701204fc3f8d31e84cf8ca272974e2be9c867](https://github.com/ps5-payload-dev/sdk/tree/4eb701204fc3f8d31e84cf8ca272974e2be9c867). Identifies LLVM 18.1.8 and the runtime configuration. It installs software if run; it is included as provenance and has not been executed by this publication update. Original author/license comments are preserved; the GPL text is supplied in the project and SDK license notices.

The portable local RADV link adaptation is `runtime-adaptations/radv-mkstemp/link_radv_prx.sh`: set PROSPERO_REPO_ROOT to matching Prospero Win source and provide the pinned RADV checkout, output directory, native conversion tool and PIE script as its four arguments. RADV_MKSTEMP_SOURCE is an optional shim override. No build was run to produce these evidence files.

See the root LICENSE_CLOSURE.md for current completed checks and remaining distribution materials. This record does not relicense upstream source or make the tested binary ZIP automatically redistributable.

The later `../license-closure/` materials complete the previously open recursive static-input map and add the Wine untracked/generated overlays missing from the tracked-file deltas. Use their reconstruction recipes and source companion instructions.
