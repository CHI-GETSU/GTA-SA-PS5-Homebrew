# Reconstruction ciblée

Base locale : R3 fonctionnelle, eboot SHA-256 `b137f76bd7fbc200ce931b439c9fe741a38d6ca51c819c3a2f516535a9be8a44`. Le pont provient de R2, avec Wine épinglé à `490f6d5dcbb2a5047345b8af88d114bbcaad69a8` et Prospero `644e32e400c555e340d507383398c5d21b39b0b0`.

Sources complètes et build isolés conservés dans `build/gta-sa-ps2-controls-r4/wine-source/` et `wine-build/`. Ils ont été copiés depuis R2 avec des fichiers indépendants, puis les chemins Makefile/config.status adaptés. Le fichier fonctionnel modifié est fourni comme `patches/joystick_xinput.c` et comme diff R3→R4. Les autres sources de la DLL sont fournies dans `wine-dinput/` du dossier de livraison.

Depuis la racine du projet :

```
make -C build/gta-sa-ps2-controls-r4/wine-build -j2 dlls/dinput8/i386-windows/dinput8.dll
i686-w64-mingw32-gcc -std=c11 -Os -Wall -Wextra -Werror -Ibuild/gta-sa-ps2-controls-r4/tools build/gta-sa-ps2-controls-r4/tools/ps2_setup.c -o build/gta-sa-ps2-controls-r4/test-bin/ps2-setup.exe -lshell32 -luser32
cc -std=c11 -O2 -Wall -Wextra -Werror -Ibuild/gta-sa-ps2-controls-r4/tools build/gta-sa-ps2-controls-r4/tests/test_settings.c -o build/gta-sa-ps2-controls-r4/test-bin/test-settings
build/gta-sa-ps2-controls-r4/test-bin/test-settings
python3 build/gta-sa-ps2-controls-r4/test_host.py
python3 build/gta-sa-ps2-controls-r4/prepare_delivery.py
python3 build/gta-sa-ps2-controls-r4/verify_delivery.py
```

Les tests de profils compilent `tests/test_profiles.c` avec `build/v0.2.0/src/pw_game_profile.c` et `pw_app_profile.c`. Leurs commandes et résultats sont intégrés à `verify_delivery.py`. Le test hôte requiert l'autorisation de socket Unix wineserver et ne contacte pas la PS5. Il utilise un préfixe séparé des données de livraison.

Les copies de sources de référence sont dans `reference/` pour l'audit ; elles ne sont pas incorporées au code de l'outil. Aucune reconstruction de l'application entière ni des composants graphiques n'est nécessaire.
