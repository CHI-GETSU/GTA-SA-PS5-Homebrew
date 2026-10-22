> Historical preparation report. For the current v0.1.0-beta publication decision, see [PUBLICATION_REVIEW.md](PUBLICATION_REVIEW.md). Original findings are retained for traceability.

# Droits et redistribution — état de vérification

Date : 21 octobre 2026. Les notices originales sont préservées dans les sous-projets et la release. Ce tableau est un contrôle de provenance et de conditions identifiées ; aucune autorisation ne couvre GTA ou les modules propriétaires de Sony.

| Composant | Régime constaté dans les fichiers locaux | État de publication |
|---|---|---|
| Sahin / lanceur et boilerplate | GPL-3.0-or-later | Notices conservées ; version livrée extraite de l’archive source contemporaine. Build complet encore à vérifier. |
| Prospero Win | LGPL-2.1-or-later ; titre combiné GPL-3.0 selon ses notices | Source de base et modifications natives incluses ; fermeture de toutes les sources correspondantes non certifiée. |
| Wine et dinput personnalisé | LGPL-2.1-or-later + licences des bibliothèques incluses | Patches et modules dinput inclus. Source amont lourde à récupérer au pin, puis vérifier la totalité des deltas R4 et des composants liés. |
| libc.prx | Shim indépendant GPL-3.0 selon les sources/README du boilerplate | Ce nom ne désigne pas automatiquement une bibliothèque Sony. Source de génération incluse ; provenance déclarée conservée, audit binaire exhaustif non prouvé. |
| Lapy | MIT selon le helper et sa notice | Source et manifeste propre à PPSA99405 conservés ; droits des dépendances SDK à respecter. |
| SDK public et plateformes PS5 | GPL-3.0-or-later, fichiers à licences propres | Sources et pins publics ; ne pas ajouter de SDK fournisseur ni firmware. |
| Mesa / RADV / Zink | Principalement MIT, autres licences par fichier ; assemblage PS5 GPL selon ses notices | Textes conservés ; correspondance des sources/liaisons et adaptation locale mkstemp à fermer. |
| LLVM / MinGW | Apache-2.0 avec exceptions LLVM et licences propres | Notices runtime conservées ; chaînes de compilation non vendoriées. |
| FreeType | FreeType License choisie, notices zlib et composants | Textes conservés ; obligation d’attribution à maintenir. |
| GnuTLS / Nettle et dépendances | LGPL-2.1-or-later / LGPL-3.0-or-later selon composants | Notices conservées ; sources liées et possibilités de reliaison à fournir selon conditions applicables. |
| Racines Mozilla / PSL | MPL-2.0 | Données publiques et licence conservées. Pas des clés privées. |
| DXVK du préfixe | Dépôt amont généralement zlib ; version locale détectée 2.6.2-prospero2 | **Vérifié** : licence zlib/libpng récupérée au tag ; les 10 DLL sont identiques à l’archive officielle épinglée. Licence ajoutée dans la release ; source du commit a6f6dcc incluse, sous-modules externes à récupérer. |
| miniz, yyjson, PicoSHA2, stb, Monocypher, QR | MIT / BSD / CC0 ou options propres selon notices | Licences vendoriées ; conserver les attributions et options choisies. |
| PacBrew / HTTPS UI | curl, Apache-2.0, zlib, BSD-3-Clause, MIT et MPL selon composant | Manifestes présents ; contenu source complet des bibliothèques statiquement liées à confirmer. |
| Polices | OFL / licences propres selon famille | Textes conservés dans assets et third_party ; aucune attribution supprimée. |
| Visuels GTA du paquet | Générés par code local, police système libre ; pas de screenshot Rockstar identifié | Script conservé ; vérifier les droits de présentation/marque avant diffusion publique. Les images étrangères à ce produit ont été écartées quand non nécessaires. |

## Points restant à fermer

1. DXVK résolu : hashes des 10 DLL, licence et source exacte dans `publication/DXVK_PROVENANCE.json` et `third-party/dxvk`. Compléter les sous-modules pour son build.
2. Fournir un ensemble cohérent de sources correspondantes (ou leur accès conforme) pour tous les binaires copyleft, y compris bibliothèques statiques et modifications locales. Une liste d’URL n’est pas une attestation de conformité.
3. Compléter/valider les recettes de runtime, de préfixe vierge et de packaging sur un environnement distinct. Les recettes historiques ne remplacent pas un build propre vérifié.
4. Confirmer toute provenance sans licence explicite, notamment les médias conservés et composants liés indirectement. Le code générant un visuel n’accorde aucun droit sur une marque commerciale.

La [FAQ officielle GNU sur les sources correspondantes](https://www.gnu.org/licenses/gpl-faq.en.html) et les textes GPL/LGPL locaux sont les références. La [release officielle Prospero Win v0.2.0](https://github.com/mpereiraesaa/prospero-win/releases/tag/v0.2.0) confirme la base, mais ne prouve ni la propreté des chemins de notre adaptation ni une équivalence des binaires.

La navigation web des licences aux tags exacts a échoué, puis le téléchargement HTTPS direct officiel a permis de vérifier DXVK et sa licence. La licence locale Sahin a été lue. Les incertitudes restantes concernent la fermeture des sources et composants indirects, pas une interdiction identifiée de DXVK. Rien n’a été publié.
