# Dépendances et provenance

Les versions ci-dessous sont celles des notices et manifestes de la livraison. Une référence publique ne démontre pas à elle seule qu’un binaire est reproductible ou exempt de chemins privés.

| Élément | Version / révision | Source / recette |
|---|---|---|
| Sahin | `01.004.000` + adaptations GTA livrées | https://github.com/7tg/sahin ; `sahin/launcher`, `sahin/LICENSE` |
| Prospero Win | `644e32e400c555e340d507383398c5d21b39b0b0` | https://github.com/mpereiraesaa/prospero-win ; `prospero-win/` |
| Wine | `490f6d5dcbb2a5047345b8af88d114bbcaad69a8` + 88 patches de base et changements R4 | https://gitlab.winehq.org/wine/wine ; `prospero-win/wine/patches`, `r4-dinput-sources`, `r4-wine` |
| SDK OpenGL interface | `b696cd39d63a4e346da3c680a31238b41ae359a8` | https://github.com/mpereiraesaa/ps5-opengl ; `ps5-opengl/dependencies.json` |
| Mesa pour interface | 26.2.0 | URL et SHA-256 dans `ps5-opengl/dependencies.json` |
| Boilerplate moteur | `9c0b994a048521af6fb84c73ded364504fe250e9` | https://github.com/mpereiraesaa/ps5-native-app-boilerplate ; `third-party/native-boilerplate` |
| Boilerplate outils PRX | `30597512539e7edfde079cbcaf4a626bc0a948c5` | même dépôt ; scripts de `prospero-win/tools` |
| SDK payload public | v0.42 pour interface/helper | https://github.com/ps5-payload-dev/sdk ; pins des bootstrap scripts |
| Lapy pour PPSA99405 | `57fcc19b7f57e46c823d6f2a6c6233ebf3288079` | https://github.com/mpereiraesaa/PS5-Lapy-JB-Daemon ; `third-party/lapy`, manifeste de la release |
| PS5_Mesa RADV | `01d70579b8d410d6a482e36c6f184798ae4c83ad` | https://github.com/mpereiraesaa/PS5_Mesa |
| PS5_Vulkan | `8aee3afb37bcfad06f0c7d263b901040f08ebbd0` | https://github.com/mpereiraesaa/PS5_Vulkan |
| PS5_PayloadSDK fork RADV | `95c08f27386fc698f6bbe21dde3030140a41d10b` | https://github.com/mihawk-99/PS5_PayloadSDK |
| Zink/WGL | PS5_Mesa `e4607ef697fe3df8d1ebc329bf1917d041843306` | `prospero-win/tools/build_mesa_zink.sh` et manifeste livré |
| llvm-mingw | `20260922-ucrt` | SHA-256 et archives dans le manifeste Mesa-Zink et la recette |
| FreeType | 2.13.3 | SHA-256 dans les notices de runtime ; recette PS5 Wine |
| GnuTLS / Nettle | 3.8.13 / 3.10.2 | `prospero-win/tools/build_tls_ps5.sh` ; dépendances libtasn1, libunistring, inih, mini-gmp à leurs pins de recette |
| Racines TLS Mozilla | extrait curl 2026-09-25 | SHA-256 livré ; données publiques, pas une clé privée |
| DXVK dans le préfixe livré | chaîne `v2.6.2-prospero2` constatée en 32/64 bits | https://github.com/mpereiraesaa/dxvk ; archive SHA-256 `c00c1cd7df7ce70e89130b61c1fcd2d3a18ac473f274a219da4478147e4f3c90` dans `pw_install.py` ; les 10 DLL ont été comparées et sont identiques ; source a6f6dcc1eaa67946482ebdded0d1d5dc13293133 incluse dans third-party/dxvk |
| Encodeur BC7 | `b9438627eef73a1157e84201b6fa6eb2ffd6d9f0` | https://github.com/richgel999/bc7enc_rdo ; outil de build, pas runtime |
| Dépendances UI | Monocypher, yyjson, miniz, PicoSHA2, stb, qrcodegen | pins et hashes dans `sahin/launcher/third_party/STORE_SOURCES.json` et `FOUNDATIONS.json` |
| Bibliothèques HTTPS UI | PacBrew v0.40.2 | `PACBREW_LICENSES.json` : curl 8.18.0, OpenSSL 3.5.2, zlib 1.3.2, zstd 1.5.6, libpsl 0.21.5, PSL |

Les sources lourdes téléchargées (Wine complet, Mesa et toolchains) ne sont pas vendoriées intégralement. Les recettes de récupération sont incluses mais la fermeture complète des sources correspondantes n’est pas certifiée. Ne pas présenter ce dossier comme une archive autonome reconstruisant hors ligne tous les binaires.

Les patches DXVK 3.1.1 de la référence Sahin sont historiques ; **ils ne décrivent pas les DLL 2.6.2-prospero2 constatées dans notre préfixe**. Il faut garder ces versions distinctes.

## Artefacts officiels contrôlés sans substitution

Prospero Win v0.2.0 : ZIP SHA-256 `7585efc624c2f8077c0d579a045ef808129a7b0bc525a9dd9c5c6db0c51626ca`. Sahin 01.004.000 PPSA99404.zip : SHA-256 `4bab6e06c0e29098fa6e4631e11e5ca742b1b30d1537192325567dd1b9a532b0`. Les deux ZIP temporaires ont été supprimés après inspection ; aucune bibliothèque n’a été substituée. Les métadonnées publiques de release et le comparatif par fichier sont dans `publication/`. Répéter le comparatif exige de télécharger à nouveau ces entrées temporaires.
