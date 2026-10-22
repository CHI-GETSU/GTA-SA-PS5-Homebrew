#!/usr/bin/env python3
"""Assemble a PPSA99604 TEST candidate; preserve every unrelated baseline byte.
SPDX-License-Identifier: GPL-3.0-or-later
"""
from pathlib import Path
import argparse,shutil,json,hashlib,zipfile
TITLE='PPSA99604'
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--runtime',type=Path,required=True,help='Read-only validated complete baseline app')
p.add_argument('--launcher',type=Path,required=True)
p.add_argument('--game',type=Path,required=True)
p.add_argument('--output',type=Path,required=True)
o=p.parse_args()
if o.output.exists():raise SystemExit('Output must not exist')
param=json.loads((o.launcher/'sce_sys/param.json').read_text())
assert param['titleId']==TITLE and TITLE in param['contentId'] and param['conceptId']=='99604'
manifest=json.loads((o.launcher/'lapy-manifest.json').read_text())
assert manifest['target_title']==TITLE and manifest['console_validated'] is False
assert manifest['protocol_sha256']=='bb02c4aa814eaba7a7a423a31b29ff41f786212c2953678cf29434e85fa0f869'
assert manifest['elf_sha256']==hashlib.sha256((o.launcher/'lapy.elf').read_bytes()).hexdigest()
for path in [o.launcher/'eboot.bin',o.game,o.launcher/'lapy.elf']:
 data=path.read_bytes();assert TITLE.encode() in data and b'PPSA99405' not in data,path
for tree in [o.runtime,o.launcher]:
 for f in tree.rglob('*'):
  assert not f.is_symlink(),f
  assert f.name.lower() not in {'gta_sa.exe','gta_sa.set','gta3.img','gta_int.img','.env','dev.conf'}
  assert not f.name.lower().startswith('gtasasf')
with zipfile.ZipFile(o.runtime/'assets/bundle/bare-prefix-sahin.zip') as z:
 assert z.testzip() is None
 assert not any(n.lower().endswith(('/gta_sa.exe','/gta_sa.set')) or Path(n).name.lower().startswith('gtasasf') for n in z.namelist())
shutil.copytree(o.runtime,o.output)
for name in ['eboot.bin','lapy.elf','lapy-manifest.json']:
 shutil.copy2(o.launcher/name,o.output/name)
shutil.copy2(o.game,o.output/'game.bin')
shutil.copy2(o.launcher/'sce_sys/param.json',o.output/'sce_sys/param.json')
p=o.output/'SOURCES.txt'
t=p.read_text().replace('PPSA99405',TITLE)
p.write_text('PPSA99604 TITLE-ID MIGRATION — LOCAL TEST BUILD, NOT VALIDATED ON PS5.\n'
 'Rebuilt launcher, native runner and own-title Lapy helper from the prepared R4 sources.\n'
 'The only runner source changes are its title ID and matching return marker.\n'
 'Wine, DirectInput/XInput, DXVK, Mesa, workers, libc, assets and the game-free prefix are reused byte-for-byte.\n'
 'No PS5 transfer or GitHub publication was performed. Source/license redistribution holds remain open.\n\n'+t)
for name in ['eboot.bin','game.bin','lapy.elf','store-worker.elf','self-updater.elf','sce_module/libc.prx']:(o.output/name).chmod(0o755)
changed=[]
for f in sorted(o.runtime.rglob('*')):
 if f.is_file():
  rel=f.relative_to(o.runtime);dest=o.output/rel
  assert dest.is_file()
  if f.read_bytes()!=dest.read_bytes():changed.append(rel.as_posix())
assert set(changed)=={'eboot.bin','game.bin','lapy.elf','lapy-manifest.json','sce_sys/param.json','SOURCES.txt'},changed
assert {f.relative_to(o.runtime) for f in o.runtime.rglob('*') if f.is_file()}=={f.relative_to(o.output) for f in o.output.rglob('*') if f.is_file()}
print(json.dumps({'title':TITLE,'changed_files':changed,'console_validated':False},indent=2))
