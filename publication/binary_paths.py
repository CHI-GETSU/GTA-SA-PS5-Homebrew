#!/usr/bin/env python3
"""Locate personal build paths and map PE/ELF sections, without rewriting bytes."""
import hashlib,io,json,re,struct,zipfile,collections
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
APP=ROOT.parent/'GTA-SA-PS5-RELEASE'
# Match a personal build root from the local environment; do not store its value.
user_home=Path.home().as_posix().encode()+b'/'
RX=re.compile(re.escape(user_home)+rb'[^\x00\r\n\"<> ]{1,500}')
def sections(b):
 out=[];fmt='unknown container'
 try:
  if b[:2]==b'MZ':
   pe=struct.unpack_from('<I',b,60)[0];assert b[pe:pe+4]==b'PE\0\0'
   symptr,symcount=struct.unpack_from('<II',b,pe+12);strtab=symptr+18*symcount
   n=struct.unpack_from('<H',b,pe+6)[0];opt=struct.unpack_from('<H',b,pe+20)[0];fmt='PE'
   for i in range(n):
    at=pe+24+opt+40*i;name=b[at:at+8].rstrip(b'\0').decode(errors='replace')
    if name.startswith('/') and name[1:].isdigit():
     pos=strtab+int(name[1:]);name=b[pos:].split(b'\0',1)[0].decode(errors='replace')
    size,off=struct.unpack_from('<II',b,at+16);out.append((off,off+size,name))
  else:
   elf=b.find(b'\x7fELF');assert elf>=0;fmt='ELF' if elf==0 else 'SELF with embedded ELF header'
   assert b[elf+4]==2
   off=struct.unpack_from('<Q',b,elf+40)[0];sz,n,idx=struct.unpack_from('<HHH',b,elf+58)
   # Wrapped SELF uses file segment mappings; ELF offsets cannot safely be assumed.
   if elf!=0:return fmt,[]
   table=elf+off;stroff,strlen=struct.unpack_from('<QQ',b,table+idx*sz+24)
   names=b[stroff:stroff+strlen]
   for i in range(n):
    at=table+i*sz;ni=struct.unpack_from('<I',b,at)[0];name=names[ni:].split(b'\0',1)[0].decode(errors='replace')
    start,size=struct.unpack_from('<QQ',b,at+24);out.append((start,start+size,name))
 except (AssertionError,struct.error,IndexError):pass
 return fmt,out
def component(name):
 n=name.lower()
 if 'dinput8.dll' in n:
  if '/i386-windows/' in n or '/syswow64/' in n:return 'R4 DirectInput custom 32-bit DLL','No verified drop-in replacement; retain custom controls source'
  return 'Wine DirectInput 64-bit','Official generic Wine 64-bit component exists; not the 32-bit R4 controller bridge'
 if 'mesa-zink' in n:return 'Mesa WGL/Zink + compiler runtime','PS5_Mesa source available; no path-clean matching binary verified'
 if n.endswith('eboot.bin'):return 'Sahin GTA launcher + static dependencies','No official build of this GTA adaptation; upstream Sahin is not a drop-in'
 if 'wine' in n or 'bare-prefix' in n:return 'Wine runtime / Wine prefix','Wine upstream source exists; standard host builds are not PS5 PRX replacements'
 return 'PS5 helper/runtime','Public source references included; no matching path-clean official artifact verified'
rows=[]
def inspect(name,b):
 hits=list(RX.finditer(b));wide=list(RX.finditer(b.replace(b'\0',b''))) if b'\0' in b else []
 if not hits and not wide:return
 if Path(name).suffix.lower() not in {'.bin','.elf','.prx','.dll','.exe','.cpl','.drv','.ocx','.acm','.sys','.dll16','.exe16'}:return
 fmt,secs=sections(b);loc=[]
 for m in hits:
  value=m.group();section=next((n for a,z,n in secs if a<=m.start()<z),'unmapped/SELF')
  source=bool(re.search(rb'\.(?:c|cc|cpp|h|hpp|l|y|rc|inc)(?::.*)?$',value))
  kind='debug metadata' if section.startswith(('.debug','/')) else ('source filename literal; likely assertion/diagnostic' if source else 'build path; use not established')
  loc.append({'offset':m.start(),'section':section,'path':value.replace(user_home,b'<BUILD_HOME>/').decode(errors='replace'),'classification':kind})
 comp,official=component(name)
 if name.endswith('libgnutls.prx'):
  comp='GnuTLS TLS runtime'
  official='GnuTLS 3.8.13 source is public; no matching path-clean PS5 PRX verified. A host library is not a drop-in.'
  for l in loc:
   if l['path'].endswith('/gnutls/config'):l['classification']='runtime configuration default; optional file open confirmed in GnuTLS priority.c'
 rows.append({'file':name,'component':comp,'sha256':hashlib.sha256(b).hexdigest(),'format':fmt,'ascii_occurrences':len(hits),'nul_stripped_occurrences':len(wide),'locations':loc,'severity':('personal build metadata plus optional runtime config path' if name.endswith('libgnutls.prx') else 'build metadata containing personal username'),'runtime_assessment':('GnuTLS lib/priority.c initializes system_priority_file with SYSTEM_PRIORITY_FILE, uses fopen, and accepts a GNUTLS_SYSTEM_PRIORITY_FILE environment override. Missing file returns success with defaults. No change made.' if name.endswith('libgnutls.prx') else 'Debug sections are non-runtime. Source filename literals may be printed by assertions/diagnostics; no filesystem dependency established by static occurrence alone. SELF section mapping remains unproven, but observed personal paths are source filename literals.'),'official_clean_replacement':official,'rebuild':'Possible from corresponding source using -ffile-prefix-map, -fdebug-prefix-map, -fmacro-prefix-map and reproducible timestamps; not performed. ABI/protocol and PS5 testing still required.'})
for p in sorted(APP.rglob('*')):
 if not p.is_file():continue
 n=p.relative_to(APP).as_posix();b=p.read_bytes();inspect(n,b)
 if zipfile.is_zipfile(io.BytesIO(b)):
  with zipfile.ZipFile(io.BytesIO(b)) as z:
   for i in z.infolist():
    if not i.is_dir():inspect(n+'!/'+i.filename,z.read(i))
comparison=ROOT/'publication/OFFICIAL_RUNTIME_COMPARISON.json'
if comparison.exists():
 official_index={x['file']:x for x in json.loads(comparison.read_text())['files']}
 for row in rows:
  o=official_index.get(row['file'])
  if o:
   row['official_comparison']=o
   if o.get('official_member'):
    row['official_clean_replacement']=o['official_member']+'; personal build-root absent='+str(o.get('personal_build_root_absent'))+'; '+o['status']
(ROOT/'publication/BINARY_PATHS.json').write_text(json.dumps(rows,indent=2)+'\n')
md=['# Chemins de compilation dans les binaires','', 'Aucun binaire modifié. Les chemins exposés sont remplacés par `<BUILD_HOME>` dans ce rapport seulement. Chaque membre ZIP est inventorié séparément. Les artefacts officiels disponibles sont comparés individuellement ci-dessous ; absence du chemin personnel ne signifie pas équivalence fonctionnelle. La présence d’un chemin ne prouve pas une dépendance de chargement.','', '| Fichier exact | Composant | Occurrences ASCII | Sections | Nature / usage | Version officielle sans chemin personnel |','|---|---|---:|---|---|---|']
for r in rows:
 kinds=collections.Counter(x['classification'] for x in r['locations']);sec=sorted({x['section'] for x in r['locations']})
 md.append('| `'+r['file']+'` | '+r['component']+' | '+str(r['ascii_occurrences'])+' | '+', '.join(sec)+' | '+'; '.join(kinds)+' | '+(r.get('official_comparison',{}).get('official_member') or 'Pas de correspondant établi')+' |')
md+=['','## Versions officielles et reconstruction','', '| Composant | Version officielle sans chemins | Reconstruction générique |','|---|---|---|']
for comp in sorted({r['component'] for r in rows}):
 group=[r for r in rows if r['component']==comp]
 mapped=sum(bool(r.get('official_comparison',{}).get('official_member')) for r in group)
 official=f"{mapped}/{len(group)} correspondants officiels sans chemin personnel, équivalence à valider"
 if 'R4 DirectInput' in comp:official+=' ; générique inadapté aux commandes R4'
 md.append('| '+comp+' | '+official+' | Possible en principe; sources correspondantes et validation à compléter. |')
md+=['','Pour chaque fichier, `publication/BINARY_PATHS.json` donne SHA-256, offsets, sections, chemins neutralisés et limites de classification. Les noms de sources dans `.rdata`/`.rodata` peuvent servir aux assertions et diagnostics exécutés; ils ne sont donc pas toujours du DWARF supprimable. Un SELF ne peut pas être classé par offsets ELF sans décoder ses tables. Aucune dépendance à un répertoire de compilation sur la console n’a été démontrée.','', 'Références : [Wine](https://gitlab.winehq.org/wine/wine), [PS5_Mesa](https://github.com/mpereiraesaa/PS5_Mesa), [Sahin](https://github.com/7tg/sahin). Aucune de ces références ne garantit un binaire de remplacement identique avec des chemins neutralisés.']
(ROOT/'CHEMINS_BINAIRES.md').write_text('\n'.join(md)+'\n')
print('binaries with personal build paths',len(rows),'unique hashes',len({r['sha256'] for r in rows}))
print(collections.Counter(r['component'] for r in rows))
