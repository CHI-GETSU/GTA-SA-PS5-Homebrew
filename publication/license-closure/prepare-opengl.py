#!/usr/bin/env python3
"""Materialize a fresh OpenGL build tree from the source companion; no compilation."""
import argparse,hashlib,json,shutil,subprocess,tarfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('companion',type=Path);p.add_argument('destination',type=Path);a=p.parse_args();c=a.companion.resolve();d=a.destination.resolve();here=Path(__file__).resolve().parent
if d.exists():raise SystemExit('Choose a new empty destination.')
# Verify immutable source archives before extraction.
pins={x['file']:x['sha256'] for x in json.loads((here/'UPSTREAM_ARCHIVES.json').read_text())}
if hashlib.sha256((c/'upstream/mesa-26.2.0.tar.xz').read_bytes()).hexdigest()!=pins['mesa-26.2.0.tar.xz']:raise SystemExit('Mesa archive hash mismatch')
d.mkdir(parents=True)
with tarfile.open(c/'project-source.tar') as t:t.extractall(d,filter='data')
gl=d/'project/ps5-opengl';deps=gl/'third_party';deps.mkdir(exist_ok=True)
for src in (c/'opengl-dependencies').iterdir():shutil.copytree(src,deps/src.name)
with tarfile.open(c/'upstream/mesa-26.2.0.tar.xz') as t:t.extractall(deps,filter='data')
shutil.copyfile(c/'upstream/mesa-26.2.0.tar.xz',deps/'mesa-26.2.0.tar.xz')
shutil.copytree(c/'overlays/mesa-26.2.0',deps/'mesa-26.2.0',dirs_exist_ok=True)
# Recreate the PSBC source index for the upstream write-tree identity check.
psbc=deps/'opengnm-psbc'
subprocess.run(['git','init','-q',str(psbc)],check=True)
subprocess.run(['git','-C',str(psbc),'add','-f','--all'],check=True)
pin=json.loads((gl/'dependencies.json').read_text())['psbc_patch']['patched_tree']
actual=subprocess.check_output(['git','-C',str(psbc),'write-tree'],text=True).strip()
if actual!=pin:raise SystemExit('Exported PSBC preferred source tree mismatch: '+actual)
# Original HEAD checks require upstream Git history; replace only those checks in
# this new build tree after checking the exported source-file manifest.
manifest=json.loads((here/'OPENGL_DEPENDENCY_FILES.json').read_text())
for e in manifest:
    q=deps/e['file']
    if not q.is_file() or hashlib.sha256(q.read_bytes()).hexdigest()!=e['sha256']:raise SystemExit('Dependency source mismatch: '+e['file'])
p=gl/'toolchain/build-opengnm-psbc.sh';s=p.read_text();start=s.index('check_revision() {');end=s.index('\n}\n',start)+3
s=s[:start]+'# Exported source contents were checked by prepare-opengl.py.\ncheck_revision() { :; }\n'+s[end:];p.write_text(s)
print(d/'project')
