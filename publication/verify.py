#!/usr/bin/env python3
"""Read-only release checks, independent of private workspace paths."""
from pathlib import Path
import hashlib,json,zipfile,sys,subprocess
root=Path(__file__).resolve().parents[1]
release=Path(sys.argv[1]) if len(sys.argv)>1 else root.parent/'GTA-SA-PS5-RELEASE'
app=release/'PPSA99405'
def sha(p):return hashlib.sha256(p.read_bytes()).hexdigest()
assert sha(app/'eboot.bin')=='4863f5e0a1bf71f6bcc97934caa0fc651f5a946e5e16fb8676da9741026e7d34'
assert sha(app/'win/wine/lib/wine/i386-windows/dinput8.dll')=='71cddcbf57913c3205a27ded31ae824f867fa536578b9a16f542ac0c5a52a0bb'
assert json.loads((app/'sce_sys/param.json').read_text())['titleId']=='PPSA99405'
m=json.loads((app/'lapy-manifest.json').read_text());assert m['target_title']=='PPSA99405';assert sha(app/'lapy.elf')==m['elf_sha256']
for n in ('game.bin','eboot.bin','lapy.elf','store-worker.elf','self-updater.elf','sce_module/libc.prx','assets/plans/GTASA.json'):
 assert (app/n).is_file(),n
assert (app/'win/wine').is_dir() and (app/'win/mesa-zink').is_dir()
with zipfile.ZipFile(app/'assets/bundle/bare-prefix-sahin.zip') as z:
 assert z.testzip() is None
 assert {'system.reg','user.reg','userdef.reg'}.issubset(z.namelist())
 for n in z.namelist():
  assert not (n.startswith('drive_c/Games/') and not n.endswith('/'))
  assert Path(n).name.lower() not in {'gta_sa.exe','gta_sa.set','gta3.img'}
  assert not Path(n).name.lower().startswith('gtasasf')
# In a development checkout, validate the tracked source tree, not its own
# legitimate Git metadata or ignored local audit/build output.
if (root/'.git').is_dir():
 records=subprocess.check_output(['git','-C',str(root),'ls-files','--stage','-z']).split(b'\0')
 for record in records:
  if not record:continue
  metadata,name=record.split(b'\t',1)
  assert metadata.split()[0] in (b'100644',b'100755'), 'symlink or submodule in source tree'
  relative=Path(name.decode());assert '.git' not in relative.parts
  assert (root/relative).is_file() and not (root/relative).is_symlink()
else:
 assert not any(p.name=='.git' or p.is_symlink() for p in root.rglob('*'))
assert not any(p.name=='.git' or p.is_symlink() for p in release.rglob('*'))
print('PASS delivered eboot, title, helper, R4 dinput, runtime layout, ZIP integrity, no game/save names, no Git history or symlinks')
