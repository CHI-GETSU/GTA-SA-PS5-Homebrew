# v0.1.0-beta — PPSA99604 final preparation

The original PPSA99604 test ZIP has now been validated on a real PS5 by the owner: launch, DualSense, AUTO2 and right-stick camera after selecting Joypad. It is frozen, with SHA-256 `4aa16e1a012d07a92f29d4607e6d13d5a440b40fba4193b8afc9bfc6575206ee`. No binaries were rebuilt during this source publication update. The old PPSA99405 delivery and both previous ZIPs remain intact.

The frozen application has 1855 files; 1849 match the old baseline. The six changed files are the three rebuilt title-specific binaries, Lapy's manifest, the shipped param.json and SOURCES.txt. Both source param.json files, catalogue and packaging target PPSA99604. All 38 native source files were compared at migration time: only the title substitutions in wine64_main.c changed; AUTO2 and the pad sources remain unchanged.

English README, credits, installation and Joypad setup instructions, release notes, frozen binary identities and recovered corresponding-source evidence are prepared for main. See [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md) for completed checks and remaining binary distribution work; source publication alone does not clear that gate.

The previously performed recursive ZIP audit covered 3665 file/member contents with no container errors, detected credential signatures, prohibited game/save names or matches against the commercial-game reference hash set. It reported non-secret build-path metadata separately. The final source Git index is audited independently before commit/push. Neither audit proves absence of arbitrary transformed commercial data or unknown secrets.

No PS5 files, saves or settings are accessed or changed. The legally supplied PC game path, shared R4 prefix and save paths remain unchanged. No graphics mod is included.

The 2026-10-22 source closure adds `publication/license-closure/`: the recursive
OpenGL static-input map, exact SDK/PacBrew sources/recipes, Wine's missing
untracked/generated overlays and source reconstruction/link materials are now
recovered. Binary publication requires the complete corresponding-source
companion alongside the immutable tested ZIP, the final asset audit, and the
separate decision about the already disclosed non-secret personal metadata.
