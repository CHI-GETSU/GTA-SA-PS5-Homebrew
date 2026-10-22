# Third-party attribution and license map

This adaptation builds on the work below. Licenses apply per component and file; the root license does not replace permissive, LGPL, font or data licenses. Exact runtime revisions are recorded in `DEPENDENCIES.md`, the runtime's `SOURCES.txt`, and its component manifests.

| Project / component | License evidence | Role and retained notices |
|---|---|---|
| [Sahin](https://github.com/7tg/sahin), tag 01.004.000 | GPL v3; per-file GPL-3.0-or-later notices where present | Adapted single-game launcher; `sahin/LICENSE`, launcher notices and source |
| [Prospero Win](https://github.com/mpereiraesaa/prospero-win), 644e32e4… | LGPL-2.1-or-later | Native runtime, profiles and Wine integration; `prospero-win/LICENSE`, `NOTICE.md`, `LICENSING.md` |
| [Wine](https://gitlab.winehq.org/wine/wine), 490f6d5d… | LGPL-2.1-or-later plus bundled component licenses | R4 DirectInput and XInput path; `r4-wine/COPYING.LIB`, per-file notices, Wine patches |
| [PS5 Native App Boilerplate](https://github.com/mpereiraesaa/ps5-native-app-boilerplate) / BlackBearReloaded | GPL v3 / per-file GPL-3.0-or-later | CRT, native container tools and independent libc shim; `third-party/native-boilerplate/LICENSE`, launcher `THIRD_PARTY_NOTICES.md` |
| [Public PS5 payload SDK](https://github.com/ps5-payload-dev/sdk), v0.42 | Upstream GPL v3 text verified at tag; component notices also apply | Build toolchain/platform prerequisites; pinned retrieval scripts. The incomplete extracted toolchain view is excluded from the Git snapshot. |
| [Lapy JB Daemon](https://github.com/mpereiraesaa/PS5-Lapy-JB-Daemon), 57fcc19… / Arksama (Team PHU) | MIT for Lapy's own code | Title-specific helper; `third-party/lapy/PS5-Lapy-JB-Daemon-57fcc19/LICENSE` |
| [ps5-agc-gears logging client](https://github.com/mpereiraesaa/ps5-agc-gears), 1ae1f918… | Upstream GPL v3 text retrieved at the exact commit | `third-party/lapy/ps5log-1ae1f918/LICENSE` and `NOTICE.md`; distinct from Lapy's MIT license |
| [ps5-opengl](https://github.com/mpereiraesaa/ps5-opengl), b696cd39… | GPL-3.0-or-later project code, upstream component licenses | Launcher graphics SDK; `ps5-opengl/LICENSE`, `THIRD_PARTY_NOTICES.md`, `LICENSES/`, immutable `dependencies.json` |
| Mesa / PS5_Mesa / PS5_Vulkan / PS5_PayloadSDK fork | Primarily MIT for Mesa; GPL platform/linking components and file-specific licenses | RADV and Zink/WGL; runtime `LICENSES/mesa`, GPL notices, local mkstemp shim source |
| [DXVK](https://github.com/mpereiraesaa/dxvk), 2.6.2-prospero2 | zlib/libpng | `third-party/dxvk/LICENSE`; exact source commit a6f6dcc… and ten runtime DLLs verified against the pinned official archive |
| LLVM / llvm-mingw / MinGW-w64 | Apache-2.0 WITH LLVM-exception and component-specific terms | Compiler/runtime notices in `prospero-win/LICENSES/` and the compiled app's `LICENSES/llvm-mingw/` |
| FreeType | FreeType License selection plus bundled notices | Runtime text rendering; compiled app `LICENSES/freetype/` |
| GnuTLS, Nettle, libtasn1, libunistring, inih, mini-gmp | LGPL and permissive terms by component | Compiled app `LICENSES/gnutls/`, `LICENSES/nettle/`; source versions and hashes in build recipes |
| Mozilla root certificates and Public Suffix List | MPL-2.0 | Public trust/data sets; not private keys. Runtime and UI notices retained. |
| Monocypher, yyjson, miniz, PicoSHA2, stb, qrcodegen | BSD/CC0, MIT, or the available upstream options as recorded | Vendored source licenses and `sahin/launcher/third_party/STORE_NOTICES.md`, `STORE_SOURCES.json`, `UI_NOTICES.md` |
| PacBrew UI HTTPS libraries: curl, OpenSSL, zlib, zstd, libpsl | curl, Apache-2.0, zlib, BSD-3-Clause, MIT; PSL MPL-2.0 | `sahin/launcher/assets/licenses/`, `third_party/PACBREW_LICENSES.json`; binary source closure still under review |
| Inter, Montserrat, DejaVu, Patrick Hand, Press Start 2P | Family-specific licenses supplied with each font | Font source/atlas notices retained under launcher assets/third_party |
| SDL-derived tile mapping and optional OpenGL example dependencies | zlib, MIT, Apache-2.0 and per-file terms | Original notices retained in native source and OpenGL license directory |
| Asset preparation scripts | GPL-3.0-or-later headers | BlackBearReloaded / Mihawk-99; upstream asset tools are retrieved separately |

GTA presentation in the compiled app is code-drawn project artwork, not extracted Rockstar game resources. Unneeded reference artwork is excluded from the staged Git tree. The application name describes compatibility; trademarks remain with their owners.

Some dependency source trees are retrieved by pinned bootstrap scripts instead of being vendored. Missing source closure is documented as a binary-publication blocker, not concealed by this table. See `PUBLICATION_REVIEW.md`.
