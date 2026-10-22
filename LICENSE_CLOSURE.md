# PPSA99604 binary redistribution review

**Source publication may proceed after the source audit. Binary Release publication is not cleared.** This review supersedes the old generic hold list. The owner has validated the unchanged PPSA99604 ZIP on PS5; runtime behavior and distribution-material completeness are separate questions. No prohibition on redistributing the reviewed source modifications was identified.

## Evidence recovered and verified

| Component | Exact evidence now available | Result |
|---|---|---|
| Native game.bin | Migrated runner source and build scripts; frozen hash in docs/VALIDATED_PPSA99604.json; 38 native files compared during migration | The native input/AUTO2 sources are preserved; only the title substitutions changed in wine64_main.c. No rebuild is needed. |
| Wine runtime PE files | WINE_PE_OUTPUT_MATCHES.json maps all **1450 shipped DLL/EXE files** to exact local build outputs | Output correspondence recovered, including stock 64-bit dinput8 from the host Wine install and the custom R4 32-bit dinput8. |
| Wine Unix PRX | CORRESPONDENCE_EVIDENCE.json matches **14 shipped PRX modules** against the original pinned Wine build report | Module hashes match, including ntdll, win32u, wowprospero and GnuTLS. RADV has separate local-adaptation evidence. |
| Wine source changes | Three full deltas from Wine commit 490f6d5dcbb2a5047345b8af88d114bbcaad69a8 | Reverse application was checked against the actual PS5, host and R4-controller compile source trees. Use one matching complete delta, not that delta plus the 88-patch series. |
| GnuTLS/Nettle | GnuTLS 3.8.13 archive ffed8ec1… and Nettle 3.10.2 archive fe9ff51c… match recipe hashes | 2994 GnuTLS and 893 Nettle source/configuration files compared; no changed archive source files found. The exact shipped TLS PRX matches its build report. Bundled libtasn1/libunistring/inih and Nettle mini-gmp remain part of the recipe's source coverage. |
| RADV libvulkan.prx | Shipped hash 3ef3f43c98459763a04f17be403ee91aa742eb09e0f92022ccd8a0981850c00c matches the original mkstemp-adaptation verification report | Local shim and link logic recovered. Portable runtime-adaptations/radv-mkstemp/link_radv_prx.sh added; syntax checked without executing a build. The original recipe is retained. |
| Lapy helper | Its compiled manifest identifies eight preferred source inputs; every hash matches vendored source | Exact source/helper target and protocol matched. Own MIT and linked ps5log GPL notices are retained; the helper is not described as MIT-only. |
| Launcher Mesa/PSBC | Mesa 26.2.0 archive hash verified; 12803 upstream members compared, 23 modified files exported; PSBC exact upstream pin and reverse patch check | Exact local upstream deltas recovered. Recipes for generated/platform sources and the OpenGL umbrella archive are included. This is more evidence than version labels alone. |
| SDK C++ support | SDK v0.42 resolved to commit 4eb701204fc3f8d31e84cf8ca272974e2be9c867; its unmodified libcxx.sh recovered | Upstream recipe identifies LLVM 18.1.8 for libunwind, libc++abi and libc++; archive pins and original copyright/license headers retained. |
| DXVK | All ten DLLs previously matched official 2.6.2-prospero2; source a6f6dcc1 and zlib/libpng notice retained | No new DXVK prohibition or binary substitution is asserted. |

Machine-readable evidence and source deltas are in [publication/final-review](publication/final-review/README.md). Build-output matching proves provenance of those files; it does not prove that every linked dependency's distribution materials have been made complete and accessible.

## Remaining concrete completion work

These are unresolved checks/materials, not a claim that the named permissive licenses automatically require a source archive or that byte-identical reproduction is legally mandatory.

| Scope / actual inputs | Remaining piece | Concrete completion route without modifying the frozen ZIP |
|---|---|---|
| Launcher OpenGL umbrella: libps5_opengl_core33.a, libglapi_bridge.a, libpsbc.ps5.a and the nested Mesa/Gallium/NIR libraries | Close the source/link manifest recursively beyond the top-level GROUP scripts: account for extra generated/platform source inputs and connect them to the exact installed archives. The upstream-member diff does not enumerate all added/generated inputs by itself. | Read the existing native-runtime and Mesa/PSBC dependency/build graphs, export the matching preferred source/generator manifest and any local additions, and publish the exact source-access/build instructions alongside the Release. No launcher recompilation is required. |
| Statically linked support used by launcher/helper/PRX: SDK libunwind.a, libc++abi.a, libc++.a, compiler-rt builtins and platform/CRT/shim inputs; PacBrew libcurl/libpsl/libssl/libcrypto/libzstd/libz | Complete the per-input source/notice/access map for the actual linkage. SDK/LLVM versions are now identified, but the entire transitive static combination and applicable notices/source coverage have not been certified. PacBrew upstream generic source versions are recorded; any port patches/recipe revision must be tied to v0.40.2 rather than assumed from a current branch. | Use the SDK's pinned source commit and LLVM 18.1.8 recipe plus the exact PacBrew v0.40.2 port recipes. Hash the relevant source/patch and license inputs, retain required notices and provide the chosen complete source-access arrangement for the covered combination. Do not list unused libraries merely because they occur in Ninja dependency tracking. |
| Applicable LGPL combinations: Wine/TLS and other linked LGPL inputs | Document the actual compliant source/reconstruction or relink route for the frozen combination, including generated PRX descriptors, emutls/platform objects and local link/conversion flags. The original sources and recipes are now substantially recovered, but the final distributed set/access arrangement is not closed. | Assemble a component-by-component source/relink manifest from the recovered deltas and scripts; include or provide the required preferred sources/materials and clear versioned retrieval instructions. If an object/relink route is chosen, supply its required usable inputs; do not imply that upstream URLs alone prove closure. |

The preserved GPL v3 and LGPL texts govern the applicable combination. No new license waiver or generic source offer is invented. Completing these records is a documentation/source-distribution task; it does not require changing the functional PS5 binaries.

## Privacy, separately from licenses

The exact frozen ZIP retains non-secret build-home paths and a prefix host-name setting. They are not passwords or API tokens and do not justify a wholesale rebuild. The earlier request to remove personal information has not been silently waived. Any public distribution decision must address those disclosed metadata while respecting the requirement to keep the exact tested ZIP. No personal values are copied into these public reports.

The immutable asset is GTA-SA-PS5-PPSA99604-TEST.zip, SHA-256 4aa16e1a012d07a92f29d4607e6d13d5a440b40fba4193b8afc9bfc6575206ee. No tag, binary upload or GitHub Release is performed while this review is open.
