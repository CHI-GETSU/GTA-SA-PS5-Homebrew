> Current PPSA99604 status: the owner reports successful PS5 testing. See [LICENSE_CLOSURE.md](../LICENSE_CLOSURE.md) and docs/VALIDATED_PPSA99604.json for the latest source/redistribution decision. The record below is historical where it describes an earlier build or review state.

> APPLIED IN THIS ISOLATED TEST BUILD. The proposal below is the preserved planning record; see ../../BUILD_REPORT.md for build results. The original prepared SOURCE and PPSA99405 payload were not changed.

# Title ID migration proposal — PPSA99604

Status: prepared, **not applied**, no compilation. The README alone labels the future public identity; the actual source/build defaults, application payload and both existing ZIPs still target PPSA99405. They must not be distributed under the proposed new folder name.

## Catalogue check

Checked on 2026-10-21 Toronto time (2026-10-22 UTC); exact timestamp and response hashes are in `CATALOG_CHECK.json`. The live JSON feed contained 104 entries. The homepage linked 113 distinct title IDs, including nine coming-soon entries absent from the feed. PPSA99405 belongs to FUTBOL13 / 7tg. **PPSA99604 was absent from the union of both lists.**

The proposal keeps the PPSA + five-digit format and the 99xxx range widely used by the homebrew catalogue, while avoiding the crowded 9940x range. It is a project proposal, not an official Sony allocation or a global reservation. Check again before publishing; do not infer availability from a missing detail page alone.

Sources: https://prosperostore.net/ ; https://prosperostore.net/catalog/v1.json ; https://prosperostore.net/app/PPSA99405/ . The catalogue source revision and all observed IDs are retained in `CATALOG_CHECK.json`.

## Exact identity changes

`identity-only.patch` is an unapplied, reviewable patch against the current working source copy. `PATCH_INPUTS.json` records the input/output hashes. `git apply --check migration/identity-only.patch` passed; no application or build was run.

| Area | Necessary changes |
|---|---|
| Native engine | `runner/native-source/wine64_main.c`: PW_TITLE_ID becomes PPSA99604; return marker becomes `/data/prospero-win/return-PPSA99604`. This changes the derived `/mnt/sandbox/PPSA99604_000/app0` and diagnostic identity while leaving control logic untouched. |
| Launcher identity/catalogue | `sahin/launcher/src/catalog/blackbox.cpp`, `.hpp`, identity comment in `src/app/store.cpp`: game identity becomes PPSA99604. GAME/BLACKBOX_GAME compile definition must agree with the catalogue and param.json. `src/main.cpp` derives return/ingame paths from that definition; it does not require hard-coded replacement. |
| Metadata | `sahin/sce_sys/param.json` and `sahin/launcher/games/PPSA99405/sce_sys/param.json`: titleId PPSA99604, contentId UP0000-PPSA99604_00-GTASANANDREAS000, conceptId 99604. Retain the title name, application version, launch intent and existing category. |
| Directory layout | After reviewing/applying the patch, rename only the source game directory `sahin/launcher/games/PPSA99405` to `sahin/launcher/games/PPSA99604`, retaining its `art` selector. Do not rename any old release payload or ZIP. New build/package outputs must use an unused, isolated candidate directory with PPSA99604 as the app folder. |
| Build wrappers | `build-launcher-local.sh`: GAME=PPSA99604. `build_runner.py`: own-title helper input under dist/PPSA99604 and native title_id output PPSA99604. `render-host-local.sh`: preview output path PPSA99604. Never reuse the old runner-stage or existing build objects as a mixed identity build. |
| Helper | `tools/build.sh` and `tools/build-lapy-helper.py` already derive the helper target from param.json; no speculative global substitution is needed. Build Lapy with TARGET_TITLE=PPSA99604. Its generated manifest must report target_title PPSA99604 and the new ELF hash. Preserve the existing helper protocol SHA-256. Never change only the manifest while retaining a helper compiled for the old title. |
| Packaging | `package_homebrew.py`: expected title PPSA99604. Add separate candidate validation of actual launcher/runner/helper identity, manifest hashes and complete runtime before accepting the output. Generate new manifests/checksums from new bytes; old hashes cannot certify the migrated binaries. |
| Tests/preview | The patch updates GTA title references in `sahin/launcher/host/store_main.cpp` and the return-path test `tests/store_early_exit_test.cpp`. Add candidate-specific checks without changing the old golden verifier `publication/verify.py` or camera probe's validated-release target. Supply a separate explicit candidate app path for future axis comparisons. |
| Documentation | README now flags the proposed identity and future `/data/homebrew/PPSA99604/` path, and warns that existing archives still use the conflicting old ID. Preserve verified Credits & Acknowledgements. After a real candidate exists, refresh build instructions, release notes, component source map and publication review with its actual identity and hashes. |
| Historical records | Retain old title IDs in historical adaptations, previous audit inputs, image metadata, comparison records and original validation reports. They describe the old binaries. `docs/prospero-blackbox-README.md` lists unrelated upstream NFS title proposals; `store_service_test.cpp` contains a generic Blur catalogue fixture. Do not present them as GTA identity assignments or rewrite third-party history wholesale. |
| Git/archive history | The public source commit and existing local beta tag remain untouched. Git history legitimately retains the old ID; do not rewrite it. Old ZIP members and central-directory paths are evidence of the baseline, not strings to hex-edit. Future candidate requires a new source revision and owner-reviewed release/tag target. |

## Minimum future compilation scope

The payload inspection found the literal old Title ID in exactly three application binaries: `eboot.bin` (one occurrence), `game.bin` (four), `lapy.elf` (one). This agrees with the source definitions. At minimum the **launcher**, **native runner** and **own-title Lapy helper** need newly built candidate versions. No byte patching or metadata-only impersonation is proposed.

Wine, the custom R4 DirectInput/XInput DLLs, DXVK, Mesa, runtime data, game-free prefix, artwork and fonts do not need rebuilding merely for this Title ID migration. The generic `store-worker.elf` and `self-updater.elf` contain no literal old Title ID; reuse is conditional on validating their parameterized app root/target handling and unchanged helper protocol, not just on string absence. Packaging may reuse their exact baseline bytes after that check.

No compilation has been performed. A new identity means new title-specific binaries: the migrated candidate must not be called PS5-validated before a separate test.

## Data paths deliberately retained

| Data | Path / identifier retained |
|---|---|
| Legally supplied classic PC game | `/data/gtasa-homebrew/base-games/GTASA/` |
| Launcher persistent data | `/data/gtasa-homebrew/` |
| Install record | `/data/gtasa-homebrew/installs/gta-sa-ps2-r4/GTASA.json` |
| Wine prefix | `/data/prospero-win/prefixes/gta-sa-ps2-r4/` |
| Installed game | `drive_c/Games/GTASA/` in that prefix |
| GTA profile | `gta-sa-homebrew`, with the existing after/input profile selection |
| Saves and settings | `drive_c/users/prospero/Documents/GTA San Andreas User Files/` in that prefix, including GTASAsf*.b and gta_sa.set |
| Optional pre-install save import | `/data/gtasa-homebrew/saves/GTASA/` |

`adopt_gta.cpp` identifies the existing game by GTASA + gta-sa-ps2-r4 + gta-sa-homebrew, not by Title ID. It checks non-empty gta_sa.exe, data/gta.dat and models/gta3.img before adopting an existing installation without a record. The ordinary already-installed check still uses its existing record behavior: do not fold the separate, unshipped installation-detection patch into this migration.

`game_install.cpp` refuses uninstall of the shared GTA/R4 prefix. Keep that guard, Install/Repair behavior, save-preservation rules and game-file detection intact. Neither real saves nor PS5 settings are read or copied by this preparation.

Title-keyed transient paths (return-PPSA99604 and ingame-PPSA99604.txt) must agree between launcher and runner. Old pending return/ingame requests must not be imported automatically. Launcher display preferences stored in maps keyed by the old ID may need a narrow GTA-only preference migration or can start at documented defaults; do not reinterpret another application's catalogue record as GTA. The game settings file gta_sa.set stays in the same Wine user folder, preserving Joypad selection.

## Regression acceptance before a future release

1. Keep the original complete PPSA99405 tree and both ZIP hashes untouched. Never automatically rename/delete `/data/homebrew/PPSA99405/`: the ID also belongs to FUTBOL13, so title ID alone is not proof of ownership.
2. Verify candidate param.json, catalogue, compile definitions, helper target/ELF hash, sandbox paths and matching return/ingame paths. Check the 1855-file baseline payload inventory; any replaced/added file must be accounted for.
3. Verify all reused runtime/DLL/prefix bytes against the baseline; preserve custom dinput8 and its override. Compare native input/AUTO2 sources to baseline with only the two identity substitutions allowed in wine64_main.c.
4. Use synthetic host fixtures for existing-prefix adoption, missing game files, Install/Repair, shared-prefix uninstall refusal and save/settings preservation. Compare before/after synthetic save hashes. A stale-record limitation remains documented.
5. After separate authorization, test the new tile on PS5: direct launch, Square menu, Install/Repair, AUTO2 at its existing timing, all buttons, both sticks, closure/return and loading/saving existing progress. Fresh-install camera test must select Joypad. No console action is part of the current task.
6. Close the component-specific source/relink gaps in LICENSE_CLOSURE.md, decide on disclosed metadata, audit the candidate and obtain final publication approval. No tag, ZIP upload or GitHub Release now.
