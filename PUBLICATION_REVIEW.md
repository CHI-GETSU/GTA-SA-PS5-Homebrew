# v0.1.0-beta publication review

## Decision

| Output | Current state | Permitted next step |
|---|---|---|
| Source on `main` | Prepared for review; no push performed | Explicit owner approval is required before the first push. |
| Local `v0.1.0-beta` tag | Prepared locally only | Do not push the tag or use it to create a public Release while the binary gate is open. |
| Compiled homebrew ZIP | Prepared locally for inspection; **binary publication blocked** | Do not upload or distribute until the corresponding-source and metadata issues below are resolved. |

This review supersedes earlier blanket preparation-status statements. Historical audits remain in the tree for traceability. The core source can be reviewed separately from the compiled application. No known prohibition in the reviewed source licenses was identified; this is not a certification that the entire compiled dependency graph has already met all distribution conditions.

## What was verified

- The destination repository was checked read-only and had no advertised refs when preparation began. No existing remote branch was overwritten.
- The source import uses a new local repository, a credential-free HTTPS remote and a public project author identity. The private parent repository's history/configuration is not imported.
- The functional PS5 executable/library bytes are preserved. No rebuild, strip, patch, signing operation or automatic console deployment was performed.
- No game executable/data, personal saves, private logs, credentials or personal configuration is intended for the staged source or ZIP. Source selection, archive checks and credential scans are recorded locally; public upstream copyright contacts are retained.
- The exact runtime DirectInput/XInput pair was tested previously; both right-stick axes passed. The README now explicitly requires first-launch Joypad selection inside GTA. AUTO2 timing is unchanged.
- The ten DXVK DLLs in the prefix were matched to the official pinned 2.6.2-prospero2 archive. Its zlib/libpng license and exact source commit are retained.
- The logging client linked into Lapy was traced to `ps5-agc-gears` commit `1ae1f9182abd2770c131b97419034fb85173c2dc`. Its GPL v3 license was retrieved at that commit and added to the source notices and compiled app's license directory. Lapy's own MIT license does not make the linked helper MIT-only.
- The public payload SDK v0.42 license was retrieved from the upstream tag. The incomplete extracted SDK/toolchain view is excluded from the Git snapshot; the verified bootstrap download remains documented.
- Unneeded upstream demo/reference pictures, raw public API caches, large per-file audit outputs and local-only preparation records are excluded from the source commit. Their removal from Git does not remove notices or modify functional PS5 binaries.

## Precise binary-publication holds

These are verification gaps, not claims that the named open-source licenses prohibit redistribution.

| Component / compiled path | Applicable condition | Evidence currently available | What must be completed |
|---|---|---|---|
| Launcher `PPSA99405/eboot.bin` and its linked OpenGL SDK | GPL v3 Corresponding Source for the covered combination, including relevant changes and build scripts | GTA launcher/graphics port sources, dependency pins and build scripts are included. The generated SDK contains libraries; a complete matched source export for all linked/fetched dependencies has not been verified. | Establish the exact complete source set (including fetched Mesa/PSBC inputs, applied changes and generated-source recipes) and provide compliant access with the binary. |
| `win/wine/lib/wine/x86_64-unix/libvulkan.prx` | GPL platform/linking code plus Mesa component notices and corresponding source where required | Mesa/PS5_Vulkan/SDK fork commits, shim source and historical local RADV linkage notes are recorded. The portable reconstruction of the exact local mkstemp-linked artifact is not fully verified. | Supply/verify the exact source+local linking adaptation and usable build/relink instructions for this artifact; do not replace it just to avoid this review. |
| Wine PS5 PRX modules, `wowprospero` and R4 DirectInput | LGPL source/combination requirements, plus licenses of linked platform/runtime code | Pinned Wine revision, 88 base patches, R4 dinput modules and native code are included or referenced. The full runtime delta/source/relink closure has not been exhaustively matched to every shipped PRX. | Verify all local deltas and dependency sources for the shipped modules and provide the source/relink access required by their applicable licenses. |
| `win/wine/lib/wine/x86_64-unix/libgnutls.prx` | LGPL obligations of the chosen GnuTLS/Nettle components and their linkage | Versions, hashes, license texts and recipes are retained; the complete preferred-form linked-source/relink set has not been certified. | Verify/provide the exact GnuTLS, Nettle and included-library sources, local changes and relinking recipe applicable to the shipped combination. |
| Lapy/SDK/logging and UI statically linked dependencies | Respective GPL, MIT and other component conditions | The missing logging GPL notice is now fixed; helper source and protocol manifest are retained. Full static dependency/source correspondence still requires closure with the above title audit. | Include linked GPL source/build coverage and preserve all library notices; do not rely only on Lapy's MIT banner. |

An independently repeated build is useful engineering validation, but byte-identical reproduction or a successful clean build is not asserted to be a separate license requirement. The hold concerns establishing/providing the actual applicable Corresponding Source and other distribution materials. Upstream URLs can be part of a compliant access arrangement; an unverified list alone is not proof of complete coverage. See the preserved GPL/LGPL texts and `LICENSING.md`.

## Metadata, separate from secrets

The unmodified compiled package retains build paths containing the original build username, source filenames used in debug/assertion messages, and a host-name setting in the game-free Wine prefix. These are **not API keys or passwords**. Most paths are debug/diagnostic metadata. GnuTLS also contains an optional system-configuration path used at runtime; missing configuration falls back to defaults.

The earlier per-binary inspection and official-runtime comparison are retained as local audit material, excluded from the source commit where bulky. Many official runtime counterparts lack the personal build path, but their bytes differ and compatibility was not established. The custom 32-bit R4 dinput must not be replaced with a generic DLL. No full rebuild is justified solely by these metadata findings.

Before binary publication, the owner must explicitly accept the disclosed non-secret metadata or a separate minimally changed candidate must be prepared and validated. Approval to publish source alone does not authorize that change or waive the binary license gate.

## Publication boundary

The first approved source push is limited to the reviewed `main` commit and its file inventory. It must not upload the compiled ZIP or create a GitHub Release. The locally prepared beta tag and compiled asset remain withheld until the binary gate is closed and an explicit release approval names the final asset/hash.

The binary ZIP is assembled from the complete validated `PPSA99405` tree, not from a selective set of runtime files. It is quarantined locally so a licensing hold does not result in a broken, stripped-down homebrew being offered to users.

This repository does not grant rights to GTA or Sony proprietary SDK/firmware material. No graphics mod is included. Nothing has been pushed, uploaded or deployed by this preparation.
