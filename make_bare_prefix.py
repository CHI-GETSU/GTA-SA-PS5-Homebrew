#!/usr/bin/env python3
"""Prepare a candidate Wine bundle from a caller-supplied clean prefix.
Never use a personal playing prefix; inspect registry values and all members.
SPDX-License-Identifier: GPL-3.0-or-later
"""
from pathlib import Path
import argparse,zipfile
p=argparse.ArgumentParser();p.add_argument('clean_prefix',type=Path);p.add_argument('output',type=Path);o=p.parse_args()
if o.output.exists():raise SystemExit('Refusing to overwrite existing bundle')
with zipfile.ZipFile(o.output,'w',zipfile.ZIP_DEFLATED) as z:
 for f in sorted(o.clean_prefix.rglob('*')):
  if not f.is_file() or f.is_symlink():continue
  n=f.relative_to(o.clean_prefix).as_posix()
  if n in {'system.reg','user.reg','userdef.reg','.update-timestamp'} or n.startswith('drive_c/windows/'):
   z.write(f,n)
 for n in ('drive_c/users/prospero/Documents/','drive_c/users/Public/Documents/','drive_c/Games/'):
  z.writestr(n,b'')
print('Candidate requires recursive privacy and provenance audit before use.')
