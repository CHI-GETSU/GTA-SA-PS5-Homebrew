#!/usr/bin/env python3
"""Restore verified Wine sources only; never configure, compile or link anything."""
import argparse, hashlib, json, shutil, subprocess, tarfile
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('variant',choices=['ps5','host','r4-controller']);p.add_argument('source_archive',type=Path);p.add_argument('destination',type=Path);a=p.parse_args()
here=Path(__file__).resolve().parent
expected=next(x['sha256'] for x in json.loads((here/'UPSTREAM_ARCHIVES.json').read_text()) if x['file']=='wine-490f6d5d.tar')
if hashlib.sha256(a.source_archive.read_bytes()).hexdigest()!=expected:raise SystemExit('Upstream Wine archive integrity failed.')
if a.destination.exists():raise SystemExit('Refusing an existing destination; choose a new directory.')
a.destination.mkdir(parents=True)
with tarfile.open(a.source_archive) as t:
    # The source archive contains a single root; extraction disallows unsafe links/paths.
    t.extractall(a.destination,filter='data')
tree=a.destination/'wine-490f6d5d'
subprocess.run(['patch','-p1','--batch','--forward','-i',str(here.parent/'final-review'/('wine-'+a.variant+'-from-490f6d5d.patch'))],cwd=tree,check=True,stdout=subprocess.DEVNULL)
overlay=here/'wine-overlays'/a.variant
for src in overlay.rglob('*'):
    if src.is_file():
        dst=tree/src.relative_to(overlay);dst.parent.mkdir(parents=True,exist_ok=True);shutil.copyfile(src,dst)
report=next(x for x in json.loads((here/'WINE_RECONSTRUCTION.json').read_text()) if x['variant']==a.variant)
for entry in report['additional_source_overlay']:
    if hashlib.sha256((tree/entry['file']).read_bytes()).hexdigest()!=entry['sha256']:raise SystemExit('Overlay integrity failed: '+entry['file'])
print(tree)
