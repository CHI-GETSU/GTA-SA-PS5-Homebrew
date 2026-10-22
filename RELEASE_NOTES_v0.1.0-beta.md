# GTA San Andreas Homebrew PS5 — v0.1.0-beta

First public beta for **PPSA99604**, distributed with the exact PS5-tested binaries, complete corresponding-source companion and retained third-party notices.

## This beta

A dedicated PPSA99604 homebrew tile using the community-adapted Sahin launcher, Prospero Win and the R4 DualSense input bridge. The project owner has tested direct launch, DualSense, AUTO2 and right-stick camera operation on a real PS5. The tested binaries are frozen; no recompilation was performed for this source/documentation publication.

The former local title PPSA99405 conflicts with FUTBOL13. Do not rename or mix the old and new title-specific binaries. The new title retains the existing GTA game, Wine prefix and save paths.

## Installation

1. Extract the complete homebrew ZIP and copy `PPSA99604/` to `/data/homebrew/PPSA99604/`. Keep every subfolder and the executable permissions, then register/refresh the tile with your homebrew loader.
2. Supply your legally obtained **classic PC GTA San Andreas** installation in `/data/gtasa-homebrew/base-games/GTASA/`, with `gta_sa.exe` directly inside that folder. No game executable, game data or personal saves are included. Definitive Edition is unsupported.
3. Hold **Square when starting the tile** to open the installation page, then use **Install**. Use **Triangle / Repair** for an existing registered installation. An existing R4 prefix can be adopted; normal launches then start the game directly.
4. On first configuration, select **GTA Options → Controller Setup → Configuration → Joypad**. This is necessary for the right-stick camera. Do not delete gta_sa.set or reset working bindings.
5. Briefly leave the sticks at rest after loading. AUTO2 was reported to activate after about three seconds; its timing and native control logic are unchanged.

The game and saves remain in `/data/prospero-win/prefixes/gta-sa-ps2-r4/`. Saved games/settings are under `drive_c/users/prospero/Documents/GTA San Andreas User Files/` in that prefix. Preserve it and avoid running both GTA tiles simultaneously. No automatic console migration or deletion is provided.

Options pauses; hold Options + touchpad click for about one second to request closure. See README.md for the full controls and limitations. The three resolution selections request a Wine desktop size; they are not texture/graphics presets or proof of GTA's actual internal render resolution. Performance overlay Off/Frame rate/Full selects diagnostics, not a speed boost.

## Frozen asset

The exact owner-tested asset retains its original filename `GTA-SA-PS5-PPSA99604-TEST.zip` (306185047 bytes).

SHA-256: `4aa16e1a012d07a92f29d4607e6d13d5a440b40fba4193b8afc9bfc6575206ee`.

The embedded test/hold notices describe its preparation state and have deliberately not been rewritten, because doing so would change the tested ZIP. See docs/VALIDATED_PPSA99604.json and PUBLICATION_REVIEW.md for the current status. A complete independent clean-host rebuild is not certified. RoSA and MMGE 3 are not included or validated.

## Credits

Thanks to **mpereiraesaa** and [Prospero Win](https://github.com/mpereiraesaa/prospero-win), **7tg** and [Sahin / Blur PS5](https://github.com/7tg/sahin), **lavavex** and the [NFS Black Box / Most Wanted PS5 contributors](https://github.com/lavavex/nfs-blackbox-ps5), and the Wine, DXVK, Mesa and PS5 homebrew tools communities. See README.md, THIRD_PARTY.md and preserved component licenses for complete attribution.

This is an independent community adaptation, without official affiliation with or endorsement by the credited developers, Rockstar Games, Take-Two Interactive or Sony. Credits do not replace license obligations.

### Corresponding source

Download the accompanying `GTA-SA-PS5-PPSA99604-CORRESPONDING-SOURCE.tar.gz` and
checksum for the preferred sources, actual local changes, source generators,
SDK/PacBrew recipes and Wine/TLS reconstruction/relink materials. Start with its
README and `project/publication/license-closure/README.md`. The source companion
and retained third-party notices are part of this distribution; the tested
binary ZIP is not the complete source distribution by itself. No executable or
library was rebuilt during the final source/licensing preparation.

The unchanged ZIP contains non-secret historical build-path and prefix host-name metadata. The owner explicitly authorized retaining them to preserve the exact tested asset. No password, API token, personal save or commercial game file is included in this distribution.
