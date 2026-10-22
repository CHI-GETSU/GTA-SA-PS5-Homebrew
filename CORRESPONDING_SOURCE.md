> Current PPSA99604 status: the owner reports successful PS5 testing. See [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md) and docs/VALIDATED_PPSA99604.json for the latest source/redistribution decision. The record below is historical where it describes an earlier build or review state.

# Binary-to-source access record — verification incomplete

**Binary publication remains blocked.** This access map supplies useful source locations; it is not a certification that every linked source, local change and relinking material is covered. Read `PUBLICATION_REVIEW.md` and the preserved GPL/LGPL texts.

The published adaptation snapshot is [828e24599b45b4f7a612490342f555d65b0f004e](https://github.com/CHI-GETSU/GTA-SA-PS5-Homebrew/tree/828e24599b45b4f7a612490342f555d65b0f004e). Its `BUILDING.md`, `DEPENDENCIES.md` and component notices describe reconstruction boundaries. This file and the new credits are local documentation changes until separately committed and published.

| Shipped component | Preferred-form source / pins | Open verification |
|---|---|---|
| `PPSA99405/eboot.bin`, workers, updater | Snapshot `sahin/launcher/`; [Sahin 01.004.000](https://github.com/7tg/sahin/tree/01.004.000); `ps5-opengl/dependencies.json`, its source hashes and patch | Enumerate the actual static SDK/UI link inputs and match all fetched preferred-form sources, changes and generated-source recipes. |
| `game.bin` | Snapshot `runner/native-source`, `build_runner.py`; [Prospero Win base](https://github.com/mpereiraesaa/prospero-win/tree/644e32e400c555e340d507383398c5d21b39b0b0) | Match the complete linked native/platform source set; documentation is not proof of a complete independent rebuild. |
| Wine PRX / R4 controller modules | [Wine revision](https://gitlab.winehq.org/wine/wine/-/tree/490f6d5dcbb2a5047345b8af88d114bbcaad69a8), snapshot `prospero-win/wine/patches`, `r4-wine`, `r4-dinput-sources` | Match all shipped local runtime changes and applicable LGPL combination/relink materials. |
| RADV `libvulkan.prx` | [PS5_Mesa](https://github.com/mpereiraesaa/PS5_Mesa/tree/01d70579b8d410d6a482e36c6f184798ae4c83ad), [PS5_Vulkan](https://github.com/mpereiraesaa/PS5_Vulkan/tree/8aee3afb37bcfad06f0c7d263b901040f08ebbd0), [SDK fork](https://github.com/mihawk-99/PS5_PayloadSDK/tree/95c08f27386fc698f6bbe21dde3030140a41d10b), snapshot `runtime-adaptations/radv-mkstemp` | Turn the historical local link adaptation into a complete, usable source/relink recipe for this exact artifact. |
| Mesa WGL/Zink | [PS5_Mesa revision](https://github.com/mpereiraesaa/PS5_Mesa/tree/e4607ef697fe3df8d1ebc329bf1917d041843306), snapshot `prospero-win/tools/build_mesa_zink.sh`; shipped `mesa-zink-manifest.json` | Retain matching source changes and notices; distinguish this Mesa revision from RADV and launcher Mesa. |
| `libgnutls.prx` | GnuTLS 3.8.13, Nettle 3.10.2 and dependencies pinned by snapshot `prospero-win/tools/build_tls_ps5.sh`; source archives and hashes in `PPSA99405/SOURCES.txt` | Match the actual source versions/options/local changes and required relinking materials for the selected linkage. |
| `lapy.elf` | [Lapy revision](https://github.com/mpereiraesaa/PS5-Lapy-JB-Daemon/tree/57fcc19b7f57e46c823d6f2a6c6233ebf3288079), snapshot `third-party/lapy`; [logging source](https://github.com/mpereiraesaa/ps5-agc-gears/tree/1ae1f9182abd2770c131b97419034fb85173c2dc) | Include the GPL logging/static dependency source coverage, not just Lapy's MIT license. |
| DXVK DLLs in the prefix | [Exact source revision](https://github.com/mpereiraesaa/dxvk/tree/a6f6dcc1eaa67946482ebdded0d1d5dc13293133), snapshot `third-party/dxvk` | Ten DLLs previously matched the pinned upstream asset; zlib/libpng notice retained. Fetch build submodules at the source revision's gitlinks when rebuilding. |

For network distribution, review GPL v3 sections 1 and 6 and LGPL 2.1 sections 4–6 with the actual linkage/access arrangement. Source references must remain accessible and complete where required. A clean or byte-identical rebuild is not asserted to be an independent license requirement. Do not invent a source offer without an arrangement to fulfill it.

No GTA game, proprietary Sony SDK/firmware, private save or personal log is licensed or supplied by this project. No executable, library or prefix bytes were changed to prepare this document.
