# GTA San Andreas Homebrew PS5

Run **Grand Theft Auto: San Andreas — classic PC edition** from a dedicated PS5 homebrew tile, powered by an adapted [Sahin](https://github.com/7tg/sahin) launcher and [Prospero Win](https://github.com/mpereiraesaa/prospero-win).

**Beta: `v0.1.0-beta` · PS5 title ID: `PPSA99405`**

> **You must legally obtain and supply your own compatible copy of the classic PC game. GTA San Andreas, its executable, game data and personal saves are not included. The Definitive Edition is not supported.**

This is an unofficial community project. It is not affiliated with or endorsed by Rockstar Games, Take-Two Interactive or Sony Interactive Entertainment.

## Features

- A dedicated GTA San Andreas homebrew tile with direct game launch after installation.
- Install and Repair from a user-supplied PC game folder.
- DualSense controls through the existing R4 DirectInput/XInput bridge.
- **AUTO2:** automatic native-control activation after a short period of stable controller reads and neutral input. The clean-console test reported about **three seconds**; this is an observation, not a configurable promise.
- Support for in-game saves and persistent GTA settings in the Wine user folder.
- Recognition of the existing R4 prefix, without replacing its saved games.

## Requirements

- A PS5 already configured with a compatible homebrew environment, folder-style application loading/registration, and the loader/elevation services required by Prospero Win. This repository does not install that environment.
- Your legally obtained, compatible classic PC GTA San Andreas installation.
- Enough storage for both the source game folder and the installed copy in the Wine prefix.

Firmware/loader combinations beyond the reported tests are not certified. When a compiled asset is available, use the project's [Releases page](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/releases). The source repository does not contain the compiled runtime.

## PS5 installation

1. Close the game and Prospero Win.
2. Copy the **complete `PPSA99405` folder** from the homebrew ZIP to:

   ```text
   /data/homebrew/PPSA99405/
   ```

   Keep `eboot.bin`, `game.bin`, `win/`, `assets/`, `sce_module/`, helpers and their dependencies together. `game.bin` is the homebrew engine, not the PC game executable. Do not rename the title folder or change its title ID.
3. Refresh/register the homebrew tile using your loader's procedure, for example ShadowMountPlus. Preserve executable permissions where required by the loader; an FTP transfer can change them.
4. Place your classic PC game files in:

   ```text
   /data/gtasa-homebrew/base-games/GTASA/
   ```

   `gta_sa.exe` must be directly inside that directory, alongside `data/`, `models/`, `audio/`, `anim/`, `text/` and the rest of your installation. Avoid `GTASA/GTASA/` nesting.
5. Hold **Square while starting the tile** to open the installation/settings page. Select **Install**. For an already registered installation, use **Triangle / Repair** to copy the source files into the existing Wine prefix.
6. Launch the game and complete the first-launch Joypad step below.

The game runs from:

```text
/data/prospero-win/prefixes/gta-sa-ps2-r4/drive_c/Games/GTASA/
```

The `base-games` directory is an installation source. Copying files there alone does not install the game; run Install or Repair. Keep that source available for later repairs. This version can still display Play when an installation record remains after the game files were moved; hold Square and run Repair in that case.

## Required first-launch step: select Joypad

On a fresh installation, open **GTA Options → Controller Setup → Configuration** and select **Joypad** instead of Mouse + Keyboard. Menu wording may vary by language.

**This initial setting is required for the right stick to control the camera in native mode.** AUTO2 enables the input bridge; it does not change GTA's own controller/camera mode. A fresh installation may otherwise have working buttons but no horizontal or vertical camera movement.

Change only the controller mode first. Do not reset bindings or delete `gta_sa.set`. Return to gameplay, allow AUTO2 to activate and test both camera directions. Use GTA's right-stick inversion setting only if the vertical direction needs adjustment. Existing users should keep their working settings.

If Joypad is already selected and the camera still does not respond, see [camera diagnostics](camera-diagnostic/README.md). The software axis tests do not replace a console test.

## DualSense controls

| Control | Action |
|---|---|
| Left stick | Player movement / vehicle steering in native control mode |
| Right stick | Camera; requires Joypad selected in GTA |
| Face buttons, D-pad and shoulder buttons | Existing R4 native PS2-style mappings, interpreted through GTA's controller settings |
| Options | Pause / Escape |
| Triangle | Back in the PC menus; native action depends on gameplay context |
| Square held while opening the tile | Installation/settings page |
| Triangle on that page | Repair a registered installation |
| R3 held + touchpad click | Manual native-input activation if needed |
| Options + touchpad click, held about one second | Request game closure |

A touchpad click alone does not turn native controls off. After loading, briefly leave the controller at rest for AUTO2. This beta keeps the existing AUTO2 timing and input system.

## Saves and settings

GTA writes its actual saved games and settings under:

```text
/data/prospero-win/prefixes/gta-sa-ps2-r4/drive_c/users/prospero/Documents/GTA San Andreas User Files/
```

`GTASAsf1.b` through `GTASAsf8.b` are saved games; `gta_sa.set` contains settings. Optional pre-install save import uses `/data/gtasa-homebrew/saves/GTASA/` and does not overwrite an existing corresponding save. That import folder is not where the game writes new saves.

The prefix can be shared with the previous R4 installation. **Do not delete or move the entire prefix to relocate the game.** Preserve your settings and back up your own saves before maintenance. Never publish personal saves or diagnostic logs.

## Current limitations

- Beta software; compatibility, performance and stability vary by game build and PS5 environment.
- Clean-console launch and automatic controls have been reported working. First-launch Joypad selection is required; complete cold-install regression coverage is still limited.
- The newer, unshipped installation-detection patch is deliberately excluded.
- The custom 32-bit `dinput8.dll` and the `dinput8=b` override are required. Do not replace them with a generic ASI loader.
- RoSA, MMGE 3 and other graphics mods are not included or validated by this beta.
- A complete rebuild on a separate clean Nobara host has not been verified. Dependency retrieval and some runtime/source-closure steps remain documented work.
- Binary publication remains blocked pending the corresponding-source/license and metadata review described in [PUBLICATION_REVIEW.md](PUBLICATION_REVIEW.md). A locally prepared ZIP is not an authorization to redistribute it.

## Source, builds and attribution

The adapted launcher is in `sahin/launcher/`; GTA native engine changes are in `runner/native-source/`; the pinned Prospero Win base, Wine patches, R4 bridge and tests are included in their respective directories. See [BUILDING.md](BUILDING.md), [DEPENDENCIES.md](DEPENDENCIES.md) and [VALIDATION.md](VALIDATION.md).

The version tag `v0.1.0-beta` identifies this community distribution. It does not change the internal PS5 application version or title ID.

The combined application uses GPL version 3 terms, while individual components retain their own licenses, including Prospero Win/Wine under LGPL-2.1-or-later. See [LICENSE](LICENSE), [LICENSING.md](LICENSING.md) and [THIRD_PARTY.md](THIRD_PARTY.md). Original copyright and attribution notices are retained. No license here grants rights to distribute GTA or proprietary Sony SDK/firmware files.
