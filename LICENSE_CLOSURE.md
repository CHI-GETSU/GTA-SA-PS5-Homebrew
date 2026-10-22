# PPSA99604 binary redistribution review

**The three source-material gaps are closed for distribution of the unchanged ZIP together with the complete corresponding-source companion, notices and checksums.** This review supersedes the old generic hold list. The owner has validated the unchanged PPSA99604 ZIP on PS5; runtime behavior and distribution-material completeness are separate questions. No prohibition on redistributing the reviewed source modifications was identified.

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

## Completion of the three remaining source-material scopes

The three previously listed material gaps have been addressed locally without
rebuilding or modifying any PS5-tested binary. The binary must not be published
alone: the complete companion source asset and its notices/instructions must be
available on the same Release. The recursive source audit passed with public upstream fixtures classified; the
owner explicitly accepted the unchanged non-secret personal metadata. The final
asset integrity check and same-Release source access are enforced before making
the Release public.

| Scope | Completed evidence and materials |
|---|---|
| OpenGL / PSBC / Mesa | 19 actual installed static archives, all 1,275 object members matched to original objects and preferred compile inputs; 23-file Mesa overlay, full dependency sources, 34 generated Mesa/runtime compile sources and 28 PSBC generated sources/headers; original generators/build flags and a source-tree preparation recipe. |
| SDK / PacBrew | Six used PacBrew archives match the v0.40.2 package; its exact recipe commit is 1687e827675c66e1b040751c7f0811222bd3aea0. Five upstream source archives match recipe hashes. Three SDK C++/unwind archives and the Wine SDK libc match official v0.42 inputs. LLVM 18.1.8, SDK CRT/platform source and recipes, all 165 actual compiler-rt builtins objects/source mappings, port recipes and notices are supplied. |
| Wine / TLS source and relinking route | Three upstream-plus-delta source reconstructions, now including the previously untracked 32/32/33-file overlays: 37,495 preferred source files compared, zero mismatches. Exact GnuTLS/Nettle archives, included dependency sources, generated PRX descriptors, matched SDK emutls source, linker scripts, shims and conversion tools/flags supplied. Alternate exact-source recipes preserve these trees rather than replacing them with only the older patch series. Complete-source reconstruction is the selected LGPL route; an object-only kit is not promised. |

See [publication/license-closure/README.md](publication/license-closure/README.md)
for versioned inputs, usable preparation/reconstruction instructions and the
source companion layout. Existing license texts and per-file notices govern;
thanks and credits do not replace these materials. No source offer is invented.
No byte-identical clean rebuild is asserted or required as a new release gate.

## Privacy, separately from licenses

The exact frozen ZIP retains non-secret build-home paths and a prefix host-name
setting. They are not passwords or API tokens. On 2026-10-22 the owner explicitly
authorized distribution of this exact ZIP with those disclosed metadata, subject
to satisfactory licensing/source-material checks. The original binaries and
prefix remain unchanged. No personal values are reproduced in these reports.

The immutable asset is GTA-SA-PS5-PPSA99604-TEST.zip, SHA-256 4aa16e1a012d07a92f29d4607e6d13d5a440b40fba4193b8afc9bfc6575206ee. The binary must be published with the complete source companion, required notices and checksums, after the final audit succeeds.
