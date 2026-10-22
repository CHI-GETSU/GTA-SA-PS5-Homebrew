# GTA San Andreas Homebrew PS5

Welcome to my GTA San Andreas PS5 homebrew project. I maintain this community adaptation so you can run **Grand Theft Auto: San Andreas — classic PC edition** from a dedicated PS5 homebrew tile, using an adapted [Sahin](https://github.com/7tg/sahin) launcher and [Prospero Win](https://github.com/mpereiraesaa/prospero-win).

**Beta: `v0.1.0-beta` · PS5 title ID: `PPSA99604`**

[Download](#download-the-ps5-release) · [Requirements](#requirements) · [Installation](#ps5-installation) · [Joypad camera setup](#required-first-launch-step-select-joypad) · [Troubleshooting](#troubleshooting) · [Documentation](#documentation)

> **I've tested this release on my own PS5.** The game launches successfully, the DualSense works with AUTO2, and the right analog stick controls the camera after selecting **Joypad** in the game's controller settings. I've kept the tested binaries unchanged. The release includes the exact ZIP I tested, its complete corresponding-source companion and third-party notices; see [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md).

> **You must legally obtain and supply your own compatible copy of the classic PC game. GTA San Andreas, its executable, game data and personal saves are not included. The Definitive Edition is not supported.**

This is an unofficial community project. It is not affiliated with or endorsed by Rockstar Games, Take-Two Interactive or Sony Interactive Entertainment.

Prospero Win provides the Windows compatibility runtime, including Wine, that lets the PC game run in this PS5 homebrew environment. I maintain the GTA-specific adaptation; the underlying runtime and launcher remain the work of their original authors and contributors.

## Features

- A dedicated GTA San Andreas homebrew tile with direct game launch after installation.
- Install and Repair from a user-supplied PC game folder.
- DualSense controls through the existing R4 DirectInput/XInput bridge.
- **AUTO2:** automatic native-control activation after a short period of stable controller reads and neutral input. In my clean-console test, activation took about **three seconds**; this is an observation, not a configurable promise.
- Support for in-game saves and persistent GTA settings in the Wine user folder.
- Recognition of the existing R4 prefix, without replacing its saved games.

## Download the PS5 release

Open [v0.1.0-beta](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/releases/tag/v0.1.0-beta) and expand **Assets**. For the PS5 application, choose **[GTA-SA-PS5-PPSA99604-TEST.zip](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/releases/download/v0.1.0-beta/GTA-SA-PS5-PPSA99604-TEST.zip)**.

| Release download | Purpose |
|---|---|
| `GTA-SA-PS5-PPSA99604-TEST.zip` | The complete PS5 homebrew folder to extract and install. This is the exact ZIP I've tested; the `TEST` filename is retained to preserve that distribution. |
| `GTA-SA-PS5-PPSA99604-CORRESPONDING-SOURCE.tar.gz` | The complete corresponding-source companion, including reconstruction materials. Keep it with the distribution; it is not the application folder to copy to the PS5. |
| `THIRD_PARTY_NOTICES-PPSA99604.zip` | The accompanying third-party notices and license materials. |
| `SHA256SUMS.txt` | Checksums for checking the downloaded distribution files. |
| GitHub's **Source code (zip / tar.gz)** | A snapshot of the tagged repository, not the compiled PS5 application or a substitute for the complete corresponding-source companion. |

Download the complete corresponding-source companion, notices and checksums alongside the application ZIP. They accompany this binary distribution; see [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md). You do not need to build the project to install the tested application.

The ZIP retains its preparation-era test/hold notices and original bytes. [PUBLICATION_REVIEW.md](PUBLICATION_REVIEW.md) and the published Release notes explain the later publication status. I've not repackaged the ZIP to update those historical notices.

## Requirements

- A PS5 already configured with a compatible homebrew environment, folder-style application loading/registration, and the loader/elevation services required by Prospero Win. This repository does not install that environment.
- Your legally obtained, compatible classic PC GTA San Andreas installation.
- Enough storage for both the source game folder and the installed copy in the Wine prefix.

I can't confirm compatibility with firmware/loader combinations beyond those tested. Download the compiled release from my [Releases page](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/releases). The source repository does not contain the compiled runtime.

## PS5 installation

1. Download the application ZIP identified [above](#download-the-ps5-release), extract it, and close the game and Prospero Win before copying files.
2. Copy the **complete `PPSA99604` folder** from that ZIP to:

   ```text
   /data/homebrew/PPSA99604/
   ```

   Keep `eboot.bin`, `game.bin`, `win/`, `assets/`, `sce_module/`, helpers and their dependencies together. `game.bin` is the homebrew engine, not the PC game executable. Do not rename the title folder or change its title ID.
3. Refresh/register the homebrew tile using your loader's procedure, for example ShadowMountPlus. Preserve executable permissions where required by the loader; an FTP transfer can change them.
4. Place your classic PC game files in:

   ```text
   /data/gtasa-homebrew/base-games/GTASA/
   ```

   `gta_sa.exe` must be directly inside that directory, alongside `data/`, `models/`, `audio/`, `anim/`, `text/` and the rest of your installation. Avoid `GTASA/GTASA/` nesting.
5. Hold **Square while starting the tile** to open the installation/settings page. Select **Install**. For an already registered installation, use **Triangle / Repair** to copy the source files into the existing Wine prefix.
6. Launch the game and complete the **[Joypad camera setup](#required-first-launch-step-select-joypad)** below. After installation, normal launches from the tile start the game directly; hold Square whenever you need the installation/settings page.

### Which folder is which?

| PS5 folder | Purpose |
|---|---|
| `/data/homebrew/PPSA99604/` | The extracted homebrew application. |
| `/data/gtasa-homebrew/base-games/GTASA/` | Your PC game files, used as the installation/repair source. |
| `/data/prospero-win/prefixes/gta-sa-ps2-r4/drive_c/Games/GTASA/` | The installed copy that the game runs from. |
| `/data/gtasa-homebrew/saves/GTASA/` | Optional pre-install save import; see [Saves and settings](#saves-and-settings). |

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

## Troubleshooting

| What you see | What to check |
|---|---|
| The homebrew tile does not appear | Check that the complete folder is at `/data/homebrew/PPSA99604/`, then follow your loader's refresh/registration procedure. Keep its executable permissions intact. |
| Install cannot find your game | Check that `gta_sa.exe` is directly in `/data/gtasa-homebrew/base-games/GTASA/`, with the rest of your classic PC installation. Avoid an extra `GTASA/GTASA/` folder. |
| Play is offered, but you moved or removed the installed game files | Restore your source game folder, hold **Square** while opening the tile, then use **Triangle / Repair**. This beta can retain an installation record after files have moved. |
| Buttons work, but the right stick does not move the camera | Select **Joypad** in GTA's own controller settings, return to gameplay and briefly leave the controller at rest for AUTO2. Follow the [first-launch steps](#required-first-launch-step-select-joypad) before changing bindings. |
| Joypad is already selected, but the camera still does not respond | Keep your existing settings. The [camera diagnostic notes](camera-diagnostic/README.md#si-joypad-ne-résout-pas-le-problème) describe further checks. Those notes include historical investigation stages; my confirmed Joypad result is summarized above. |
| The input bridge does not activate automatically | Briefly leave the controller at rest. **R3 held + touchpad click** requests manual native-input activation if needed. |

Do not replace the custom `dinput8.dll`, delete `gta_sa.set`, reset working bindings or remove the shared Wine prefix as a troubleshooting shortcut. Back up your own saves before maintenance. If you report a problem, describe your firmware/loader, PC game edition, installation stage and whether Joypad is selected. Keep personal saves and raw diagnostic logs private.

## Current limitations

- Beta software; compatibility, performance and stability vary by game build and PS5 environment.
- I've tested PPSA99604 launch, DualSense, AUTO2 and the camera on my own PS5. First-launch Joypad selection is required. This is not certification for every firmware, loader or PC game revision.
- The newer, unshipped installation-detection patch is deliberately excluded.
- The custom 32-bit `dinput8.dll` and the `dinput8=b` override are required. Do not replace them with a generic ASI loader.
- RoSA, MMGE 3 and other graphics mods are not included or validated by this beta.
- A complete rebuild on a separate clean Nobara host has not been verified. Dependency retrieval and some runtime/source-closure steps remain documented work.
- Download both the homebrew ZIP and the corresponding-source companion from [v0.1.0-beta](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/releases/tag/v0.1.0-beta). The complete source and license materials accompany the frozen binary distribution.

## Credits & Acknowledgements

I'd like to thank the developers and contributors whose work makes my adaptation possible:

- **[mpereiraesaa](https://github.com/mpereiraesaa)** and the [Prospero Win contributors](https://github.com/mpereiraesaa/prospero-win), for the compatibility engine that runs Windows games on PS5.
- **[7tg](https://github.com/7tg)** and the [Sahin / Blur PS5 contributors](https://github.com/7tg/sahin), for the single-game launcher on which my adaptation is based.
- **[lavavex](https://github.com/lavavex)** and the [NFS Black Box / Need for Speed: Most Wanted PS5 contributors](https://github.com/lavavex/nfs-blackbox-ps5), including the [launcher project](https://github.com/lavavex/nfs-blackbox-launcher), for their contribution to the PS5 PC-game homebrew ecosystem. This acknowledgement does not claim that their code is included here.
- The [Wine contributors](https://gitlab.winehq.org/wine/wine) ([GitHub mirror](https://github.com/wine-mirror/wine)), the [DXVK contributors](https://github.com/doitsujin/dxvk) and the [PS5 DXVK fork contributors](https://github.com/mpereiraesaa/dxvk).
- The [Mesa contributors](https://gitlab.freedesktop.org/mesa/mesa), the [PS5 OpenGL port contributors](https://github.com/mpereiraesaa/ps5-opengl), the [public PS5 payload SDK contributors](https://github.com/ps5-payload-dev/sdk), and the wider PS5 homebrew tools community.

GTA-SA-PS5-Homebrew is an **independent community adaptation**. It has no official affiliation with, endorsement by, or support agreement with the developers credited above, Rockstar Games, Take-Two Interactive or Sony Interactive Entertainment. Project and game names belong to their respective owners.

These thanks supplement the component copyright and license notices; they do not replace them. Verified upstream references are recorded in [docs/CREDITS_VERIFICATION.md](docs/CREDITS_VERIFICATION.md).

## Documentation

| If you want to… | Read |
|---|---|
| Install and play the published beta | [Download](#download-the-ps5-release), [PS5 installation](#ps5-installation) and [Joypad camera setup](#required-first-launch-step-select-joypad) above |
| Investigate a camera issue | [Camera diagnostics](camera-diagnostic/README.md) (historical investigation notes, in French) |
| Understand what was tested | [VALIDATION.md](VALIDATION.md) and [PUBLICATION_REVIEW.md](PUBLICATION_REVIEW.md) |
| Explore source and build requirements | [BUILDING.md](BUILDING.md) and [DEPENDENCIES.md](DEPENDENCIES.md) |
| Find corresponding-source and licensing materials | [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md), [CORRESPONDING_SOURCE.md](CORRESPONDING_SOURCE.md), [LICENSING.md](LICENSING.md) and [THIRD_PARTY.md](THIRD_PARTY.md) |
| Check upstream credit references | [Credit verification](docs/CREDITS_VERIFICATION.md) |

Some technical records describe earlier preparation or diagnostic stages. They are retained as evidence; they do not imply that this documentation update changed or rebuilt the PS5-tested ZIP.

## Source, builds and attribution

The adapted launcher is in `sahin/launcher/`; GTA native engine changes are in `runner/native-source/`; the pinned Prospero Win base, Wine patches, R4 bridge and tests are included in their respective directories. See [BUILDING.md](BUILDING.md), [DEPENDENCIES.md](DEPENDENCIES.md) and [VALIDATION.md](VALIDATION.md).

I've published [v0.1.0-beta](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/releases/tag/v0.1.0-beta) as the first public beta of my community adaptation. This release version does not change the internal PS5 application version or title ID.

The combined application uses GPL version 3 terms, while individual components retain their own licenses, including Prospero Win/Wine under LGPL-2.1-or-later. See [LICENSE](LICENSE), [LICENSING.md](LICENSING.md) and [THIRD_PARTY.md](THIRD_PARTY.md). Original copyright and attribution notices are retained. No license here grants rights to distribute GTA or proprietary Sony SDK/firmware files.

