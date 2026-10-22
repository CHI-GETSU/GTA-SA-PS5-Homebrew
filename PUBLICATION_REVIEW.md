# PPSA99604 publication decision

The owner validated direct launch, DualSense, AUTO2 and right-stick camera after
selecting Joypad on a real PS5. The exact frozen ZIP and binary hashes are in
`docs/VALIDATED_PPSA99604.json`. No console-side hashes were retrieved and no
validated binary was rebuilt or modified during licensing preparation.

The three remaining source-material gaps have been closed by the corresponding
source companion, source/recipe recovery and explicit input correspondence in
[publication/license-closure/README.md](publication/license-closure/README.md).
The Release must provide that complete companion and its notices/checksums beside
the binary ZIP. The recursive source scan covered 228,709 file/member contents;
its public upstream test fixtures are classified in AUDIT_PUBLICATION.md.

On 2026-10-22 the owner explicitly authorized distribution of the exact tested
ZIP with its disclosed non-secret build paths and prefix host-name metadata.
This does not authorize publishing credentials, commercial GTA content, personal
saves, private logs, vendor SDKs or firmware. No such material was detected in
the reviewed distribution.

The final source commit must be the target of v0.1.0-beta; the former local tag
pointing to the earlier PPSA99405 source snapshot must not be published. The
validated ZIP retains its original TEST filename and embedded preparation-era
notices to preserve its exact bytes. Current Release notes supersede those
historical holds. A new clean-host compile or extra PS5 test is not claimed.
