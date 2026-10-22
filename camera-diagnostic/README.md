# Diagnostic ciblé du joystick droit — installation PS5 neuve

Le retour utilisateur confirme une installation sur PS5 entièrement nettoyée : lancement fonctionnel, commandes AUTO2 activées automatiquement en environ **3 secondes**, aucun mouvement de caméra au stick droit. C’est un nouvel état de validation, distinct des anciens essais dans le préfixe existant.

**Aucun délai AUTO2 modifié. Aucun exécutable PS5 reconstruit, remplacé ou transféré. La release reste intacte.** Ce dossier fournit un diagnostic et un essai correctif de configuration, pas un correctif binaire prétendument validé sur PS5.

## Résultat des comparaisons

Les 1 720 fichiers exécutables/bundles/runtime contrôlés dans RELEASE sont identiques à la livraison locale désignée comme fonctionnelle dans le récapitulatif. La copie effectivement sur PS5 n’a pas été récupérée ni hashée. `DELIVERED_COMPONENTS.json` contient le détail.

- Le lecteur `pw_pad_ps5.c`, le convertisseur `pw_wine_display.c`, le garde AUTO2 et les fonctions de contrôle/hybrid sont identiques à l’ancien AUTO2. Le diff du moteur principal ne porte que sur l’identité du titre, l’autostart et le cycle de retour/fermeture : aucun changement d’axes.
- Le dinput R4 32 bits livré a pour SHA-256 `71cddcbf57913c3205a27ded31ae824f867fa536578b9a16f542ac0c5a52a0bb`. Il est identique à la précédente R4. Le XInput livré est également identique au build de test R4.
- Les DLL présentes dans le ZIP du préfixe ont des hashes différents des DLL complètes du runtime, mais sont identiques aux anciens placeholders du préfixe : cette différence n’est pas apparue dans la release publique. L’override `dinput8=b` reste identique.
- Le profil généré localement possède les mêmes champs `[input]` et `[debug]` que l’ancien profil R4 : XInput, `mouse = right_stick`, vitesse 900 et les quatre variables du pont. Le profil réellement créé sur la nouvelle PS5 n’a pas été récupéré.

## Chaîne des axes

| Étape | Contrôle | Conclusion / limite |
|---|---|---|
| ScePad | `right_stick.x/y` lus depuis le dernier échantillon connecté | Code inchangé ; test avec échantillons injectés passé. Pas de capture matérielle de la nouvelle console. |
| Conversion native | RX de -32768 à 32767, RY avec conversion verticale prévue | Neutre et quatre directions testés avec le véritable lecteur et convertisseur, UBSan : PASS. |
| Publication XInput | Copie de l’état complet lorsque le garde natif a basculé | Code inchangé. La télémétrie existante permet de contrôler `sent_raw` et `right_source`. |
| DirectInput | RX → `lZ`, RY → `lRz` ; `Rx/Ry` réservés aux gâchettes | Les DLL PE32 identiques à la release ont passé le test réel sous Wine hôte : quatre directions, neutre, boutons, heartbeat et reconnexion. Z et Rz sont énumérés. |
| Profil | Mêmes champs d’entrée et variables que la R4 | Comparaison sémantique PASS ; ne remplace pas la récupération du profil PS5 effectif. |
| GTA | Consomme Z/Rz pour le stick droit et choisit sa voie caméra selon son mode de contrôleur | Dernière étape à vérifier sur la console. Référence de fonctionnement : [Pad.cpp](https://github.com/gta-reversed/gta-reversed/blob/master/source/game_sa/Pad.cpp) et [MenuManager.cpp](https://github.com/gta-reversed/gta-reversed/blob/master/source/game_sa/MenuManager.cpp), pas une preuve de version de l’exécutable PS5. |

## Piste d’installation neuve — pas encore confirmée

Aucun `gta_sa.set` personnel n’est embarqué et aucun réglage GTA n’est créé par Install/Repair. Une installation neuve laisse GTA générer ses valeurs par défaut. La référence du jeu initialise son mode sur **souris + clavier**, avec la voie caméra souris activée. Le mode `[input] xinput` du profil Prospero ne sélectionne pas à lui seul **Joypad dans GTA**.

Le moteur coupe volontairement l’émulation de souris du stick droit après activation native, pour éviter la double commande. Il fournit alors les axes DirectInput. Si GTA est resté en mode caméra souris, cette association peut expliquer l’immobilité du stick droit après AUTO2. Cela n’est pas une panne du garde ni un délai trop court.

Cette explication reste une **hypothèse à départager par le menu et les traces réelles**. Un exemplaire historique de réglages PS5 v7 est structurellement valide, mais ses offsets d’options ne sont pas déduits du format v6. Le fichier v6 des tests est synthétique et n’a pas été pris pour la configuration PS5 fonctionnelle. Aucun réglage personnel n’a été copié dans ce dossier.

## Essai correctif minimal — aucun transfert de binaire

1. Lancer normalement la tuile et laisser AUTO2 s’activer comme actuellement, environ trois secondes.
2. Ouvrir Options → Configuration contrôleur. Relever le mode actuel.
3. **S’il est sur souris + clavier**, sélectionner uniquement **Joypad / Manette**. Ne pas restaurer les valeurs par défaut, ne pas réaffecter les boutons, ne pas modifier les axes ou leur inversion pour ce premier essai.
4. Revenir en jeu, CJ à pied hors cinématique, déplacer le stick droit à fond horizontalement, puis verticalement. Vérifier ensuite déplacement, actions déjà fonctionnelles et pause.
5. Quitter normalement, relancer par la tuile et contrôler que Joypad et la caméra persistent. La modification attendue est un réglage de contrôleur enregistré par GTA ; aucune sauvegarde de partie, aucun profil ni lancement direct ne doit être remplacé.

Si la caméra revient, cet essai confirme la cause de configuration. Le correctif minimal est alors cette sélection dans le jeu, accompagnée d’une instruction de premier lancement ; aucun rebuild du moteur ne se justifie.

**Si Joypad est déjà sélectionné, ne pas attribuer la panne à gta_sa.set et ne pas remplacer ce fichier.** Ne pas installer une ancienne configuration personnelle ni supprimer les réglages. Passer aux traces ci-dessous.

## Si Joypad ne résout pas le problème

Après AUTO2, maintenir chaque direction du stick droit pendant quatre secondes afin que la télémétrie périodique la voie. Relever l’heure du test, puis récupérer manuellement :

- tous les `session-0.log` à `session-7.log` sous `/data/prospero-win/logs/` ; sélectionner la session par l’en-tête, pas par le numéro ;
- `/data/prospero-win/profiles/gta-sa-homebrew.profile` ;
- `/data/prospero-win/prefixes/gta-sa-ps2-r4/drive_c/users/prospero/Documents/GTA San Andreas User Files/gta_sa.set`.

Ces éléments sont privés : ne pas les ajouter à la publication. Aucune sauvegarde GTASAsf n’est nécessaire.

| Observation dans la session | Étape à examiner |
|---|---|
| `PW_PADDIAG R2 native ... sticks=LX,LY/RX,RY` : RX/RY ne changent pas | Lecture ScePad effective, connexion/échantillons. |
| RX/RY natifs changent, `sent_raw=0` ou `right_source=mouse` persiste après l’activation annoncée | Transmission/garde effectifs de cette session ; vérifier sans modifier le délai. |
| RX/RY natifs changent et la trace de poll DirectInput reste à zéro sur le stick droit | DLL XInput réellement chargée et état reçu ; sa trace `xinput module=` donne le chemin. Vérifier notamment une DLL locale au jeu. |
| Les sticks droit XInput lus par DirectInput changent, mais GTA ne bouge pas | Mode caméra GTA, propriétés/énumération Z/Rz consommées par le jeu, profil et version effective de l’exécutable. Les traces actuelles de poll sont antérieures à la conversion en Z/Rz ; ne pas les confondre avec une mesure de ce que GTA lit. |

## Outils locaux

`inspect_settings.py FICHIER` est strictement en lecture seule : version, taille, structure des tables et marqueurs. Il **ne décode pas le mode du format v7 par des offsets supposés** et n’écrit rien.

`test_right_axes.c` teste les échantillons natifs ; `test_dinput_axes.c` teste les véritables DLL de la livraison via le build Wine hôte R4. `run_host_probe.py` utilise un préfixe synthétique séparé et exige ce build hôte de référence. Les résultats sans chemins personnels sont dans `TEST_RESULTS.json`. Les exécutables hôtes, le préfixe synthétique et les journaux bruts ont été retirés après les tests.

Le test de pont hôte confirme les axes logiciels, pas le comportement d’une PS5 réelle ni celui de GTA. La correction sur console reste à confirmer. Aucun mod graphique n’est concerné.
