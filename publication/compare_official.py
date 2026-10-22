#!/usr/bin/env python3
"""Read official upstream ZIP as data; never replace validated binaries."""
from pathlib import Path
import zipfile,hashlib,json,re
root=Path(__file__).resolve().parents[1];archive=root/'publication/official-runtime-check.zip'
assert hashlib.sha256(archive.read_bytes()).hexdigest()=='7585efc624c2f8077c0d579a045ef808129a7b0bc525a9dd9c5c6db0c51626ca'
rows=json.loads((root/'publication/BINARY_PATHS.json').read_text());out=[];personal=Path.home().as_posix().encode()+b'/'
with zipfile.ZipFile(archive) as z:
 names=set(z.namelist());cache={}
 for r in rows:
  name=r['file'];rel=name.split('PPSA99405/',1)[1];candidate=None
  if rel.startswith('win/wine/'):candidate='PPSA99995/'+rel
  elif '!/' in rel:
   member=rel.split('!/',1)[1];base=Path(member).name
   arch='i386-windows' if '/syswow64/' in member.lower() else 'x86_64-windows'
   candidate='PPSA99995/win/wine/lib/wine/'+arch+'/'+base
  result={'file':name,'official_release':'v0.2.0','archive_sha256':'7585efc624c2f8077c0d579a045ef808129a7b0bc525a9dd9c5c6db0c51626ca','official_member':None,'status':'No corresponding official artifact established for this adapted component'}
  if candidate in names:
   if candidate not in cache:
    b=z.read(candidate);cache[candidate]={'sha256':hashlib.sha256(b).hexdigest(),'personal_build_root_absent':personal not in b and personal not in b.replace(b'\0',b''),'other_build_home_present':bool(re.search(rb'/home/[A-Za-z0-9_.-]+/',b))}
   result.update(cache[candidate]);result['official_member']=candidate;result['byte_identical']=result['sha256']==r['sha256']
   result['status']='Official component exists and personal build-root check completed; ABI/behavior equivalence not established'
   if 'dinput8.dll' in name and ('/i386-windows/' in name or '/syswow64/' in name):result['status']='Official generic dinput8 exists; NOT a replacement for custom R4 controller bridge'
  out.append(result)
(root/'publication/OFFICIAL_RUNTIME_COMPARISON.json').write_text(json.dumps({'source':'https://github.com/mpereiraesaa/prospero-win/releases/tag/v0.2.0','files':out},indent=2)+'\n')
print('official mapped',sum(x['official_member'] is not None for x in out),'of',len(out),'personal paths absent',sum(x.get('personal_build_root_absent',False) for x in out),'identical',sum(x.get('byte_identical',False) for x in out))
