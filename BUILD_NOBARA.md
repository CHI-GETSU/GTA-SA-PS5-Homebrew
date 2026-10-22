# Compilation sur Nobara Linux

Les tests hôtes ont été exécutés dans l’environnement de travail Nobara. Aucun build intégral sur une installation Nobara vierge ni aucun remplacement des binaires validés n’a été effectué. Les recettes de publication sont donc préparées, pas certifiées reproductibles.

## Outils

Installer avec le gestionnaire de paquets de Nobara les outils correspondant aux noms suivants : Git, GNU Make, Bash, Python 3, Clang/LLD/LLVM 18, compilateurs C/C++ hôtes, CMake, Ninja, Meson, pkg-config, flex, bison, autoconf, automake, libtool, MinGW-w64 32/64 bits, glslang, SPIR-V Tools, zlib-devel, libcurl-devel et bibliothèques EGL/OpenGL pour les previews. Les noms exacts de paquets varient selon la version de Nobara ; vérifier leur disponibilité. Les tests hôtes ont utilisé les commandes `clang`/`clang++` du système ; le natif PS5 cible Clang 18.

Pour les dépendances Python de Mesa/Zink, utiliser un environnement virtuel et `prospero-win/tools/mesa_zink_requirements.txt` (Mako 1.3.10, MarkupSafe 3.0.4, Meson 1.9.1, packaging 25.0, PyYAML 6.0.3). Le générateur d’art utilise Pillow et une police DejaVu système. Ne pas recopier un environnement virtuel personnel dans le dépôt.

## Tests sans jeu ni console

```bash
python3 test_launcher.py
```

Les tests construisent dans `host-tests/` et utilisent des données synthétiques. LSan a échoué dans le bac à sable à cause de son traçage, puis le même test a passé hors bac à sable avec détection de fuites. Le rapport ne confond pas cet échec d’environnement avec un défaut du lanceur.

## Interface

```bash
bash sahin/launcher/tools/setup-native-dependencies.sh
bash build-opengl-local.sh
bash finish-opengl-local.sh
bash build-launcher-local.sh
```

Les scripts trouvent leur racine par leur propre emplacement. Variables : `PS5_CLANG` (défaut `clang-18`), `BUILD_JOBS`, `PS5_PAYLOAD_SDK`, `PS5_NATIVE_APP_TEMPLATE`, `PS5_OPENGL_PREFIX`, `PS5_ASSETS_TOOL`, `TMPDIR`. Ajouter LLVM 18 au PATH si Nobara l’installe sous `/usr/lib64/llvm18/bin`. Les caches `.deps/` se recréent par les bootstrap scripts épinglés.

Le snapshot SOURCE ne contient pas de `bare-prefix-sahin.zip` précompilé : créer un **préfixe Wine vierge**, installer les composants libres épinglés, puis utiliser `make_bare_prefix.py CLEAN_PREFIX OUTPUT.zip`. Auditer intégralement le ZIP et le placer sous `sahin/launcher/assets/bundle/` pour un nouveau candidat. Les registres ne doivent provenir d’aucun préfixe de jeu personnel. Cette recette de création propre reste à valider ; ne pas utiliser un préfixe personnel pour contourner ce prérequis.

Les médias GTA sont dessinés par `make_art.py`. Les médias d’exemples OpenGL étrangers au produit ont été omis ; leurs builds de démonstration ne sont pas couverts par la préparation. Les codes et notices des exemples sont conservés.

## Moteur

Après compilation du lanceur et de son Lapy pour PPSA99405 :

```bash
python3 build_runner.py
```

Le script crée `build/runner-stage` et y superpose les sources natives GTA sur la base Prospero Win. Il exige un répertoire neuf, compile tous les objets et réutilise le **helper compilé pour notre identité**, pas le helper générique d’un autre titre. Le moteur construit se trouve sous `build/runner-stage/dist/PPSA99405/eboot.bin` ; il doit devenir `game.bin` d’un candidat.

Le runtime Wine/DXVK/Mesa/TLS se reconstruit séparément avec les recettes de `prospero-win/tools/` et les pins de `DEPENDENCIES.md`. Le module manette exige `r4-dinput-sources/patches/joystick_xinput.c` et les sources `r4-wine/dlls/`. Les scripts R4 historiques attendent encore un Wine configuré et un préfixe synthétique : ces prérequis ne sont pas inclus sous forme de builds. La recette de reconstruction complète reste un point ouvert de l’audit.

## Reproductibilité future, sans toucher aux binaires actuels

Pour un futur candidat uniquement, injecter dans **chaque chaîne de compilation concernée** :

```text
-ffile-prefix-map=<source-root>=/usr/src/gtasa-homebrew
-fdebug-prefix-map=<source-root>=/usr/src/gtasa-homebrew
-fmacro-prefix-map=<source-root>=/usr/src/gtasa-homebrew
```

Utiliser `SOURCE_DATE_EPOCH`, une locale fixe, archives ordonnées et dates normalisées, outils/pins identiques, sans `.env`, `dev.conf` ou journal personnel. `-fdebug-prefix-map` seul ne retire pas les noms utilisés par `__FILE__` dans les assertions. Pour GnuTLS, contrôler aussi `--sysconfdir`/`SYSTEM_PRIORITY_FILE` : le chemin de configuration est une donnée fonctionnelle distincte de DWARF.

Ces options sont documentées et n’ont pas été appliquées aux binaires de la release. Aucun nettoyage binaire ni strip/re-signature n’a été exécuté.
