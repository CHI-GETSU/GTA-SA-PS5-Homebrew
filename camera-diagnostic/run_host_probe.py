#!/usr/bin/env python3
# SPDX-License-Identifier: LGPL-2.1-or-later
"""Probe existing shipped DLLs in a disposable, game-free host prefix.
No GTA executable or personal user files are copied. The reference build is read-only.
"""
from pathlib import Path
import os,subprocess,shutil,json,hashlib,sys
source=Path(__file__).resolve().parents[1]
ref=Path(os.environ.get('R4_TEST_ROOT',str(source.parent.parent/'gta-sa-ps2-controls-r4')))
work=source/'build/camera-diagnostic';prefix=work/'prefix'
if prefix.exists():
 if '--reuse-managed-prefix' not in sys.argv:raise SystemExit('Managed prefix exists; pass --reuse-managed-prefix to retry')
else:
 (prefix/'drive_c').mkdir(parents=True);(prefix/'dosdevices').mkdir()
 for n in ['system.reg','user.reg','userdef.reg','.update-timestamp']:
  p=ref/'host-test-prefix'/n
  if p.is_file():shutil.copy2(p,prefix/n)
 subprocess.run(['cp','--reflink=auto','-a',str(ref/'host-test-prefix/drive_c/windows'),str(prefix/'drive_c/windows')],check=True)
 (prefix/'dosdevices/c:').symlink_to('../drive_c');(prefix/'dosdevices/z:').symlink_to('/')
for n in ['tmp','runtime','cache','logs']:(work/n).mkdir(exist_ok=True,mode=0o700)
app=source.parent/'GTA-SA-PS5-RELEASE/PPSA99405'
for n in ['dinput8','xinput1_3']:
 assert (app/f'win/wine/lib/wine/i386-windows/{n}.dll').read_bytes()==(ref/f'wine-build/dlls/{n}/i386-windows/{n}.dll').read_bytes()
env=dict(os.environ,WINEPREFIX=str(prefix),WINEARCH='win64',USER='prospero',LOGNAME='prospero',
 WINEDLLOVERRIDES='winemenubuilder.exe=d;dinput8=b;xinput1_3=b',WINEDEBUG='-all,+pwpad,+loaddll',
 PW_DINPUT_XINPUT='1',PW_DINPUT_GTA_PS2='1',PW_DINPUT_SESSION='host-r2-test',
 PW_TEST_PAD_FILE=str(prefix/'drive_c/pw-test-pad.bin'),LD_PRELOAD=str(ref/'test-bin/title-sink.so'),
 XDG_CACHE_HOME=str(work/'cache'),XDG_RUNTIME_DIR=str(work/'runtime'),TMPDIR=str(work/'tmp'))
with (work/'logs/probe.log').open('w') as log:
 p=subprocess.run([str(ref/'wine-build/wine'),str(work/'test-dinput-axes.exe'),'enabled'],env=env,cwd=work,stdout=log,stderr=log,timeout=90)
subprocess.run([str(ref/'wine-build/server/wineserver'),'-k'],env=env,timeout=10)
text=(work/'logs/probe.log').read_text(errors='replace')
print('\n'.join(x for x in text.splitlines() if x.startswith(('PASS','FAIL'))))
if p.returncode:print(text[-3000:])
raise SystemExit(p.returncode)
