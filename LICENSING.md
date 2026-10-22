# Licensing and distribution boundaries

The root `LICENSE` contains GPL version 3. The combined native titles and launcher are distributed under the applicable GPL v3 terms inherited from Sahin, the native boilerplate and linked GPL components. This does not relicense independent components or remove their original notices/options.

- Prospero Win's own compatibility code and Wine-derived changes remain available under **LGPL-2.1-or-later**, as recorded in `prospero-win/LICENSE`, `prospero-win/LICENSING.md` and per-file SPDX notices.
- Sahin, the native boilerplate, public payload SDK and OpenGL project retain their upstream GPL notices.
- Lapy's own code is **MIT**. Its linked `ps5log` client comes from `ps5-agc-gears` at the pinned commit and is governed by that project's **GPL v3** license. The entire linked helper must not be advertised as MIT-only.
- Mesa, LLVM, fonts, DXVK, TLS and UI dependencies retain the licenses listed in `THIRD_PARTY.md` and their own notice files.
- Locally adapted source is marked by the project documentation and existing per-file license notices. Original authors retain their copyrights. No contributor identities or copyright contact addresses have been removed merely to hide upstream attribution.

Source code and binary distribution are separate review scopes. Sharing source under its existing licenses does not by itself satisfy every requirement for distributing the already compiled application. GPL v3 section 1 defines Corresponding Source; section 6 addresses distribution of object code. LGPL components also have source/relinking requirements applicable to their particular combination. The project must establish and provide the relevant sources, modifications, scripts and access before clearing a binary release.

The v0.1.0-beta source-material completion and selected source reconstruction/relink route are documented in `publication/license-closure/README.md` and `LICENSE_CLOSURE.md`. The complete corresponding-source companion, notices and checksums must accompany the unchanged binary ZIP on the same Release. This completion applies to that reviewed combination; it does not automatically clear future dependency or binary changes.

The [GPL v3 text](https://www.gnu.org/licenses/gpl-3.0.html) and [LGPL 2.1 text](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html), together with the preserved component licenses, are the primary references.

GTA game files, personal saves, proprietary SDKs and firmware are outside the project's license grants and must not be added to the repository or release assets. Project names identify compatibility and attribution; they do not imply endorsement.
