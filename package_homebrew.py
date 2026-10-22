#!/usr/bin/env python3
"""Assemble an explicit candidate, never overwrite the validated publication.
SPDX-License-Identifier: GPL-3.0-or-later
"""
from pathlib import Path
import argparse,shutil,json,hashlib
p=argparse.ArgumentParser();p.add_argument('--runtime',type=Path,required=True,help='Game-free complete runtime app root')
p.add_argument('--launcher',type=Path,required=True);p.add_argument('--game',type=Path,required=True)
p.add_argument('--output',type=Path,required=True);o=p.parse_args()
if o.output.exists():raise SystemExit('Output must not exist')
if not (o.runtime/'win').is_dir():raise SystemExit('Missing win/')
if json.loads((o.launcher/'sce_sys/param.json').read_text())['titleId']!='PPSA99405':raise SystemExit('Wrong title')
for tree in (o.runtime,o.launcher):
 for f in tree.rglob('*'):
  if f.is_symlink():raise SystemExit('Symlink input refused')
  if f.name.lower() in {'gta_sa.exe','gta3.img','gta_sa.set'} or f.name.lower().startswith('gtasasf'):raise SystemExit('Game/save input refused')
shutil.copytree(o.runtime,o.output)
for name in ('eboot.bin','store-worker.elf','self-updater.elf','lapy.elf','lapy-manifest.json'):
 shutil.copy2(o.launcher/name,o.output/name)
for name in ('assets','sce_sys'):
 shutil.copytree(o.launcher/name,o.output/name,dirs_exist_ok=True)
shutil.copy2(o.game,o.output/'game.bin')
print('Candidate only: recursively audit archives, licenses, privacy and test on console before redistribution.')
