> Current PPSA99604 status: the owner reports successful PS5 testing. See [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md) and docs/VALIDATED_PPSA99604.json for the latest source/redistribution decision. The record below is historical where it describes an earlier build or review state.

> Update: a subsequent user test on a fully cleaned PS5 confirmed launch and AUTO2 activation in about three seconds. First-launch Joypad selection is required for the right-stick camera. No new compiled binary was produced for that test. Earlier statements below describe their original test scope.

# Validation de cette préparation

Date : 21 octobre 2026. Aucun transfert, publication ou modification de la console.

Tests hôtes exécutés sur les copies SOURCE :

- Lanceur : installation synthétique neuve, Repair, adoption du préfixe, exclusion des injecteurs/mods, préservation des sources, sauvegardes et réglages : **PASS**.
- Profils générés et sortie anticipée du lanceur : **PASS**.
- Profil généré consommé par le véritable parseur moteur R4 et voie Options/Échap : **PASS**.
- Confinement des chemins du worker hors préfixe partagé et stockage de référence : **PASS**.
- Tests compilés avec AddressSanitizer / UndefinedBehaviorSanitizer. LSan a signalé son incompatibilité avec le traçage du bac à sable ; relance hors bac à sable réussie, sans changement du code pour masquer l’échec.

Les exécutables hôtes, objets et journaux contenant des chemins du poste ont été retirés après validation ; seul ce résultat synthétique est conservé. Aucun binaire PS5 existant n’a été modifié. Les archives de sources téléchargées ont été lues comme données, pas exécutées.

Contrôles de provenance : eboot livré `4863f5e0a1bf71f6bcc97934caa0fc651f5a946e5e16fb8676da9741026e7d34`, helper PPSA99405 et DLL dinput personnalisée vérifiés ; manifeste d’intégrité établi. Les dix DLL DXVK ont été comparées à l’archive officielle dont le SHA-256 est épinglé dans la recette de base : **identiques**.

Limites : pas de nouveau test PS5, pas de build complet depuis une Nobara vierge, pas d’installation sur console vierge, pas d’audit dynamique de tout le code privilégié, pas de garantie d’absence de vulnérabilité inconnue. Ces limites sont distinctes des résultats positifs des tests hôtes.
