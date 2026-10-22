> Current source publication audit: PPSA99604 source migration, English README/credits and corresponding-source evidence were scanned recursively before publication. No critical secret signatures, prohibited GTA/save filenames, known commercial-data hash matches or archive errors were found. Added evidence uses generic/relative paths; upstream public copyright notices remain. The frozen binaries and ZIP are unchanged, and their hashes were rechecked. See publication/final-review/SOURCE_AUDIT_SUMMARY.json and LICENSE_CLOSURE.md. The older report below is retained as historical evidence, not a claim that the owner-tested PPSA99604 build remains unvalidated.

> Current PPSA99604 status: the owner reports successful PS5 testing. See [LICENSE_CLOSURE.md](LICENSE_CLOSURE.md) and docs/VALIDATED_PPSA99604.json for the latest source/redistribution decision. The record below is historical where it describes an earlier build or review state.

> Release follow-up: see [RELEASE_PREPARATION.md](RELEASE_PREPARATION.md). The original source snapshot is public; binary distribution remains blocked. The historical report below is preserved.

> Historical preparation report. For the current v0.1.0-beta publication decision, see [PUBLICATION_REVIEW.md](PUBLICATION_REVIEW.md). Original findings are retained for traceability.

# Audit de publication — GTA San Andreas Homebrew PS5

Date : 21 octobre 2026. **Audit terminé ; publication suspendue.** Aucun transfert PS5, aucune publication GitHub, aucune recompilation ou modification d’un binaire PS5 existant. Les sources originales, le paquet de référence, le jeu, le préfixe et les sauvegardes de console n’ont pas été modifiés. Les fichiers préparés sont confinés aux dossiers RELEASE et SOURCE. Les tests ont utilisé uniquement des données synthétiques temporaires, supprimées en fin de test.

## Décision par dossier

| Dossier | Résultat | Points ouverts |
|---|---|---|
| GTA-SA-PS5-RELEASE | Structure complète prête à copier, binaires livrés conservés ; **pas prêt à partager** | Métadonnées de compilation identifiantes encore présentes par choix de préservation ; un nom de machine dans le registre du préfixe ; fermeture de sources/licences indirectes incomplète. |
| GTA-SA-PS5-SOURCE | Sources séparées et nettoyées, README GitHub, dépendances et tests ; **pas encore certifié publiable** | Recettes complètes de runtime/préfixe et sources correspondantes à fermer ; vérification des droits indirects et médias à finaliser. Aucun secret personnel confirmé dans les sources nettoyées. |

Les chemins de compilation ne sont pas assimilés à des secrets. Leur présence seule ne justifie pas une reconstruction complète. Le refus de publication respecte la demande de nettoyage strict et les points de conformité encore ouverts, sans conclure que le homebrew fonctionnel est compromis.

## Version sélectionnée et intégrité

Le paquet provient de la livraison complète avec l’eboot SHA-256 `4863f5e0a1bf71f6bcc97934caa0fc651f5a946e5e16fb8676da9741026e7d34`. L’archive de sources contemporaine a pour SHA-256 `f654df430db5bf3e1fdf19295e8fd2222ece04edc6ec1e86ad0086ac2195545e`. Elle contient la création des dossiers source/import et précède la correction locale de détection non livrée. Cette correction est exclue des deux dossiers.

L’identifiant PPSA99405, le helper et son manifeste, le dinput R4 personnalisé, `game.bin`, le runtime et le bundle ont été contrôlés. Tous les exécutables, bibliothèques et médias conservés sont identiques aux fichiers livrés ; seules des notices textuelles ont été nettoyées/actualisées et la licence DXVK ajoutée. Les dumps de diagnostic non nécessaires ont été retirés de la copie. Aucun strip, patch, changement de signature, substitution ou recompilation de ces binaires.

La validation PS5 vient du retour utilisateur : tuile, commandes, sauvegarde et Repair dans le préfixe existant. Les hashes de la copie réellement installée sur PS5 n’ont pas été mesurés. L’installation sur console vierge n’a pas été testée.

## Périmètre et méthode

Inventaire récursif et SHA-256 de chaque fichier régulier ; inspection du ZIP de préfixe et des fichiers gzip de validation ; contrôle d’intégrité des archives, membres, chemins dangereux, liens, chiffrement et formats non pris en charge. Les archives téléchargées temporairement pour DXVK, Prospero Win officiel et Sahin officiel ont été vérifiées, lues comme données et retirées après comparaison ou extraction des sources/licences nécessaires. Aucun contenu téléchargé n’a été exécuté.

Recherche de chemins locaux, noms d’utilisateur, adresses de courriel, réseaux privés, clés privées, signatures de tokens de fournisseurs, URL avec identifiants et littéraux de mots de passe/API. Analyse des chaînes ASCII et UTF-16 lisibles dans les binaires, correspondance des offsets aux sections PE/ELF quand possible. Pour SELF, les sections ne sont pas supposées identiques aux offsets ELF : cette limite figure dans l’inventaire.

Les rapports d’audit ne recopient pas les valeurs de secrets potentiels. Les chemins de compilation y sont neutralisés par `<BUILD_HOME>`. Les emails d’auteurs et identités publiques amont ne sont pas supprimés des notices de copyright. Ils ne sont pas les coordonnées privées de l’utilisateur. Les adresses réseau d’exemples et noms de compte génériques sont distingués des paramètres personnels.

Aucun dossier `.git`, historique, remote privé, `.env` effectif, configuration de déploiement privée, `.aws`, `.ssh`, préfixe personnel ou journal privé n’est inclus. Les `.gitmodules` publics décrivent des dépendances ; ils ne sont pas des métadonnées Git personnelles. Les sorties de tests contenant les chemins du poste ont été supprimées. Les métadonnées de 22 images ont été inspectées : aucun tag EXIF ni métadonnée personnelle candidate détectée. Les deux captures de démonstration OpenGL ont aussi été inspectées visuellement. Les métadonnées JSON et les registres du préfixe ont été inspectés lorsque lisibles ; les médias conservés ont été comparés au périmètre de livraison. Une inspection textuelle ne garantit pas l’absence de texte personnel dessiné dans chaque pixel d’un média tiers.

## Classification des résultats

| Gravité / nature | Constat | Traitement |
|---|---|---|
| Secret critique | Aucune clé privée, clé API, token réel, URL avec mot de passe ni identifiant personnel d’accès confirmé par les recherches effectuées. | Aucune rotation de secret ne se justifie sur ces seuls résultats. Ce constat n’est pas une preuve mathématique d’absence. |
| Information personnelle faible | Un nom de poste apparaît sous `ComputerName` dans `assets/bundle/bare-prefix-sahin.zip!/system.reg`. Propriétaire et organisation sont vides ; l’utilisateur Wine est le compte générique `prospero`. | Valeur non reproduite ici. Bundle préservé ; neutralisation de cette donnée de préfixe à prévoir dans un candidat distinct, sans recompiler les exécutables. |
| Métadonnée de compilation avec nom d’utilisateur | **1 054 fichiers ou membres binaires concernés, 553 contenus SHA-256 distincts**, doublons entre runtime et préfixe inclus. Il s’agit de 1 054 fichiers/membres, pas de 1 054 clés ou comptes. | Rapport individuel `CHEMINS_BINAIRES.md` et `publication/BINARY_PATHS.json` dans SOURCE. Aucun binaire modifié. |
| Métadonnée de débogage | Chemins DWARF dans `.debug_info`, `.debug_line`, `.debug_str`, `.debug_line_str`. | Non utilisés pour trouver le jeu ou charger les bibliothèques. Un retrait futur du débogage est ciblable, mais ne supprimerait pas les chaînes d’assertion. |
| Information technique exécutée en diagnostic | Noms de sources dans `.rdata`/`.rodata`, parfois chemins de messages d’assertion et de mpg123. | Peuvent être imprimés pendant l’exécution ; aucune dépendance d’ouverture de fichier n’en découle. Pas des secrets. Pas de remplacement automatique par Wine générique. |
| Chemin fonctionnel compilé | `win/wine/lib/wine/x86_64-unix/libgnutls.prx` contient un défaut de configuration finissant par `/etc/gnutls/config`. | GnuTLS `lib/priority.c` initialise `system_priority_file`, effectue `fopen`, accepte `GNUTLS_SYSTEM_PRIORITY_FILE` et tolère un fichier absent. Chemin réellement fonctionnel mais optionnel ; distinct du débogage. Aucun défaut de jeu démontré. |
| Simple information technique | Chemins des builds officiels, emails d’auteurs, URL amont, IP d’exemple, certificats publics et signature publique du tag DXVK. | Préservés ou neutralisés dans les notices si superflus ; aucune clé privée déduite d’un certificat public ou d’une signature. |

Le détail par binaire donne : fichier exact et membre ZIP, composant, hash, offsets/sections, nature de chaque chemin, statut du remplacement officiel et possibilité de reconstruction générique. Les chemins de sources ont été classés statiquement ; cela ne remplace pas la preuve de chaque branche de code à l’exécution. Le cas GnuTLS a été relié à la source exacte de la version construite.

## Jeu et sauvegardes

Aucun `gta_sa.exe`, archive GTA IMG, texture TXD, modèle DFF, script commercial ni fichier de sauvegarde/réglage GTA n’a été retenu. Le bundle est une base Wine : `drive_c/Games/` est vide. Les noms de fichiers du jeu présents dans les sources, tests et plans sont des références d’installation, pas leurs contenus.

Un index SHA-256 de **422 fichiers d’au moins 64 octets** de la copie locale du jeu a été utilisé pour comparer tous les fichiers publiés et membres d’archives inventoriés : aucun contenu correspondant. Les petits fichiers sont contrôlés par noms et provenance ; les contenus triviaux communs ne sont pas utilisés comme preuve de propriété commerciale. Ce contrôle ne permet pas d’identifier une éventuelle ressource commerciale transformée arbitrairement ; il est complété par la sélection des sources et l’exclusion de tout dossier de jeu.

Aucune sauvegarde personnelle ni `gta_sa.set` ; aucun journal console. Les imports facultatifs de sauvegardes sont décrits mais vides. Les anciens médias et profils Blur et les médias d’exemples non nécessaires à la préparation GTA ont été écartés quand identifiés ; les notices et codes de référence restent attribués.

## Licences et droits

Textes et attributions Sahin, Prospero Win, Wine, boilerplate/SDK, Lapy, Mesa, LLVM, FreeType, GnuTLS/Nettle, bibliothèques UI/HTTPS, polices et données publiques conservés. Le titre combiné relève du régime GPL indiqué par les projets ; les composants gardent leurs licences propres. `libc.prx` est décrit par ses sources comme un shim indépendant, pas une copie de libc Sony ; aucun module de firmware ou SDK propriétaire n’a été ajouté.

DXVK a été vérifié au-delà de son nom : les dix DLL 32/64 bits correspondent exactement à l’archive officielle `v2.6.2-prospero2`, SHA-256 `c00c1cd7df7ce70e89130b61c1fcd2d3a18ac473f274a219da4478147e4f3c90`. La licence zlib/libpng a été récupérée et ajoutée. Source du commit `a6f6dcc1eaa67946482ebdded0d1d5dc13293133` incluse ; sous-modules externes à compléter pour sa compilation.

**Restent ouverts** : ensemble complet de sources correspondantes des composants copyleft/statiques, exactitude des recettes des deltas runtime, préfixe propre et reliaison lorsque requise ; droits des composants ou médias indirects sans preuve explicite suffisante. Les URL/pins et notices ne sont pas une attestation juridique ni un substitut à ces obligations. Voir `THIRD_PARTY_PUBLICATION.md` dans SOURCE et la [FAQ GNU](https://www.gnu.org/licenses/gpl-faq.en.html). Aucune interdiction de redistribution de DXVK n’a été identifiée ; son point de licence a été résolu.

## Sécurité et portabilité

Les tests hôtes du lanceur (installation, Repair, adoption, préservation), profils et sortie anticipée, parseur moteur avec Options/Échap, et confinement des chemins du worker ont passé avec ASan/UBSan. Le premier échec LSan provenait du traçage du bac à sable ; relance hors bac à sable réussie. Les artefacts de test ont ensuite été retirés des sources à publier.

Les scripts principaux utilisent leur emplacement ou des variables d’environnement. Les anciens scripts dépendant de l’arbre de laboratoire ont été conservés comme documentation historique et ne sont pas annoncés comme reconstruisant depuis zéro. Les dépendances lourdes et certains sous-modules doivent être récupérés au pin. Syntaxe Python et scripts Bash principaux contrôlés. Aucun build complet sur une Nobara distincte n’a été validé.

L’application comporte par conception un helper privilégié, un file worker, un updater et du code de développement hérité. Les points d’entrée et URL amont sont identifiés dans les sources ; la présence de l’updater n’autorise pas une mise à jour d’un autre titre. La commande de test de self-update héritée référence encore le produit d’origine et reste un point de revue de durcissement avant publication. Aucune mise à jour ni commande distante n’a été exécutée. Pas de certificat d’audit de sécurité dynamique, pas d’analyse antivirus disponible, et pas de revue exhaustive de CVE attestée : aucun de ces contrôles n’est prétendu passé.

## Solution la moins risquée

1. Garder la version PS5 validée et tous ses binaires intacts ; c’est ce qui a été fait.
2. Fermer d’abord les sources, licences, sous-modules et recettes. Les lacunes de dossier source ne nécessitent pas de recompilation du jeu fonctionnel.
3. Traiter les noms de sources comme métadonnées à divulguer ou à neutraliser, pas comme secrets critiques. Une politique de publication acceptant ces métadonnées permettrait techniquement de garder ces binaires ; elle n’a pas été présumée acceptée ici.
4. Si le nettoyage strict est maintenu, préparer plus tard un candidat séparé : neutralisation des seuls registres de préfixe concernés, traitement ciblé du débogage lorsque le format le permet, et uniquement les composants nécessaires pour les chemins `__FILE__`. Les binaires officiels ne sont substituables qu’après preuve d’absence de chemins et d’équivalence ABI/protocole. La release officielle Prospero Win v0.2.0 a été téléchargée et son SHA-256 vérifié : 1 052 fichiers/membres ont un correspondant officiel sans le chemin personnel du poste. Aucun de ces correspondants n’est identique octet pour octet ; l’équivalence de fonctionnement n’est pas établie. Le dinput R4 32 bits est personnalisé et ne doit pas être remplacé par le dinput officiel générique. La distribution Sahin 01.004.000 a également été inspectée : son eboot vise un autre produit et aucun self-updater.elf distinct correspondant n’y a été trouvé. Voir publication/OFFICIAL_RUNTIME_COMPARISON.json dans SOURCE.
5. Si un composant doit être reconstruit, utiliser `-ffile-prefix-map`, `-fdebug-prefix-map`, `-fmacro-prefix-map`, pins et dates fixes ; pour GnuTLS contrôler aussi le chemin de configuration. Une reconstruction de tout le homebrew n’est pas exigée par le seul constat des chemins.
6. Auditer et tester tout candidat modifié séparément, puis valider sur PS5 avant de le qualifier de version fonctionnelle publique. Aucun de ces changements n’a été effectué dans cette préparation.

Les dossiers restent distincts et non publiés. La préparation s’arrête ici : aucun mod graphique n’est téléchargé ou installé.

## 2026-10-22 — recovered corresponding-source materials

The dedicated license-closure review has recovered the source material gaps
listed in LICENSE_CLOSURE.md. No executable/library was rebuilt, altered or
installed on PS5. The actual R4 source trees, AUTO2, saves and Wine prefix were
not modified. New source exports, manifests and recipes are confined to the
publication copy and a separate LICENSE-CLOSURE-PPSA99604 work directory.

The first recursive source-companion pass read 228,523 file/member contents
(5,657,913,965 bytes including containers). It found zero matches against the
422-file commercial reference hash set, zero prohibited GTA/save/Git metadata
names, zero owner-home-path matches and zero provider-token matches. Its 425
private-key markers belong to the unchanged public OpenSSL/GnuTLS/Nettle test
and example sources; 45 credential-URL matches belong to unchanged upstream
source tests/documentation. They are public fixture data, not owner credentials,
and are retained with the upstream preferred sources. Author attribution and
license contacts are preserved as public copyright metadata.

The sole container decode exception is the unchanged upstream zstd test input
`tests/gzip/hufts-segv.gz`, a malformed gzip regression fixture in the SHA-pinned
zstd 1.5.6 source archive. Its bytes were scanned; it cannot be represented as a
valid decompressed archive. It is classified as public regression input, not an
uninspected personal archive. A final pass must cover additions made after that
initial inventory and the final source asset/index. No general absence-of-secrets
claim is inferred from pattern scanning alone.

The unchanged frozen binary ZIP retains the previously disclosed non-secret
build-home paths and prefix host-name metadata. Publication of those personal
metadata remains a separate explicit decision; this source update does not
silently waive the owner's earlier confidentiality instruction.

The owner explicitly authorized retaining the unchanged tested ZIP with the previously disclosed non-secret personal metadata on 2026-10-22, conditional on a satisfactory license/source audit. No secret or commercial-content exception was authorized.

Final source-content pass: **228,709 regular file/member contents**,
**5,770,931,966 bytes** including container layers. Findings remain exactly the
425 public test-key markers, 45 public URL examples and one malformed public
zstd gzip regression fixture described above. No new source, owner-path, token,
commercial-reference or prohibited-file finding was introduced. The source
addition set also passed the existing printable/UTF-16 signature scanner with
zero findings/errors. Full raw evidence remains local; public summaries and
source reconstruction manifests are in publication/license-closure. Original
generated source whitespace is preserved for source correspondence.

The unchanged binary ZIP also passes ZIP member CRC verification; all three
validated binary hashes match. Final packaging verifies source companion member
hashes against its inventory and excludes the source-verification worktrees.
The source companion and license notices are required companion Release assets.
