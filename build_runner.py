#!/usr/bin/env python3
"""Full native compilation in a disposable staging tree; no reused object files.
Publication adaptation 2026-10-21. Not yet validated on a clean Nobara host.
SPDX-License-Identifier: GPL-3.0-or-later
"""
from pathlib import Path
import os,shutil,subprocess
root=Path(__file__).resolve().parent
stage=root/'build/runner-stage';stage.parent.mkdir(exist_ok=True)
if stage.exists():raise SystemExit('Use a fresh build/runner-stage; refusing to merge stale objects')
shutil.copytree(root/'prospero-win',stage)
shutil.copytree(root/'runner/native-source',stage/'native',dirs_exist_ok=True)
helper=root/'sahin/launcher/dist/PPSA99405'
for n in ('lapy.elf','lapy-manifest.json'):
 if not (helper/n).is_file():raise SystemExit('Build the GTA launcher and its own-title helper first')
p=stage/'tools/build_native.sh';text=p.read_text()
start=text.index('helper_download=$(mktemp ')
end=text.index('helper_magic=',start)
text=text[:start]+'''lapy_helper_elf="$GTA_HELPER/lapy.elf"
helper_download="$GTA_HELPER"
'''+text[end:]
text=text.replace('cp "$helper_download/release.json" "$build/lapy-helper-release.json"','true # helper provenance is supplied by its own-title manifest')
text=text.replace('title_id=PPSA99995','title_id=PPSA99405')
text=text.replace('644e32e4','644e32e4-gtasa-homebrew-r4')
p.write_text(text)
env=dict(os.environ,GTA_HELPER=str(helper),PS5LOG_DEV_CONF=str(root/'build/no-private-dev.conf'))
subprocess.run(['bash',str(p)],env=env,check=True)
print('Find freshly compiled eboot.bin under build/runner-stage/dist; use it as game.bin only in a new candidate package.')
