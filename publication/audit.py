#!/usr/bin/env python3
"""Offline recursive publication inventory. Does not execute scanned content.
Reports locations and categories, never matched credential values.
This heuristic scanner is not an antivirus or proof of absence of secrets.
"""
import argparse, collections, gzip, hashlib, io, json, re, stat, tarfile, zipfile
from pathlib import Path
PATTERNS={
 'host_home':rb'(?:/home/|/Users/)[A-Za-z0-9_.-]+(?:/[^\x00\r\n\"<> ]*)?',
 'windows_user':rb'[A-Za-z]:[\\/]+Users[\\/]+(?!Public\b|prospero\b)[A-Za-z0-9_.-]+',
 'private_network':rb'\b(?:192\.168\.\d{1,3}\.\d{1,3}|10\.\d{1,3}\.\d{1,3}\.\d{1,3}|172\.(?:1[6-9]|2\d|3[01])\.\d{1,3}\.\d{1,3})\b',
 'private_key':rb'-----BEGIN (?:RSA |EC |OPENSSH |DSA )?PRIVATE KEY-----',
 'provider_token':rb'\b(?:AKIA[A-Z0-9]{16}|gh[pousr]_[A-Za-z0-9]{30,}|github_pat_[A-Za-z0-9_]{40,}|sk-(?:proj-)?[A-Za-z0-9_-]{30,}|xox[baprs]-[A-Za-z0-9-]{20,})',
 'credential_url':rb'https?://[^\s/\x00:@]{1,80}:[^\s/\x00@]{1,200}@',
 'credential_literal':rb'(?i)(?:api[_-]?key|access[_-]?token|auth[_-]?token|password|passwd|secret[_-]?key)\s*[:=]\s*[\"\x27][^\"\x27\r\n]{6,}[\"\x27]',
 'email':rb'\b[A-Za-z0-9._%+-]{1,64}@[A-Za-z0-9.-]{1,253}\.[A-Za-z]{2,24}\b',
}
RX={k:re.compile(v) for k,v in PATTERNS.items()}
BAD_NAME=re.compile(r'(?i)(?:^|/)(?:gta[_-]?sa\.exe|gta3\.img|gta_int\.img|gta\.dat|GTASAsf\d\.b|gta_sa\.set|\.env|\.git|\.aws|\.ssh|credentials|session-\d+\.log)(?:/|$)|\.(?:txd|dff|gxt|ifp|scm|ipl|ide)$')
class Scanner:
 def __init__(self):self.entries=[];self.findings=[];self.errors=[];self.total=0
 def add(self,path,category,count=1):self.findings.append({'path':path,'category':category,'count':count})
 def scan(self,name,data,depth=0):
  self.total+=len(data)
  self.entries.append({'path':name,'bytes':len(data),'sha256':hashlib.sha256(data).hexdigest()})
  if BAD_NAME.search(name):self.add(name,'prohibited_name')
  # Search printable ASCII and UTF-16 strings; do not decode/execute PE/SELF.
  strings=re.findall(rb'[\x20-\x7e]{6,}',data)
  strings += [x.replace(b'\x00',b'') for x in re.findall(rb'(?:[\x20-\x7e]\x00){6,}',data)]
  strings += [x.replace(b'\x00',b'') for x in re.findall(rb'(?:\x00[\x20-\x7e]){6,}',data)]
  hay=b'\n'.join(strings)
  for k,rx in RX.items():
   if k=='email':
    # Only short windows around @; bounds prevent quadratic scans of base64 data.
    windows=[hay[max(0,m.start()-65):m.start()+256] for m in re.finditer(b'@',hay)]
    n=sum(len(rx.findall(w)) for w in windows)
   else:n=len(rx.findall(hay))
   if n:self.add(name,k,n)
  if depth>8:self.errors.append({'path':name,'reason':'nesting limit'});return
  try:
   if data[:4] in (b'PK\x03\x04',b'PK\x05\x06'):
    with zipfile.ZipFile(io.BytesIO(data)) as z:
     seen=set()
     for i in z.infolist():
      if i.filename in seen:self.add(name+'!/'+i.filename,'duplicate_archive_member')
      seen.add(i.filename)
      if i.filename.startswith('/') or '..' in Path(i.filename).parts:self.add(name+'!/'+i.filename,'unsafe_archive_path')
      if stat.S_ISLNK(i.external_attr>>16):self.add(name+'!/'+i.filename,'archive_symlink')
      if i.flag_bits&1:raise ValueError('encrypted member')
      if i.is_dir():continue
      if i.file_size>1024**3:raise ValueError('member exceeds 1 GiB limit')
      self.scan(name+'!/'+i.filename,z.read(i),depth+1)
   elif data[:2]==b'\x1f\x8b':self.scan(name+'!/gzip',gzip.decompress(data),depth+1)
   elif len(data)>262 and data[257:262]==b'ustar':
    with tarfile.open(fileobj=io.BytesIO(data)) as t:
     for i in t:
      if i.issym() or i.islnk():self.add(name+'!/'+i.name,'archive_symlink')
      if i.isfile():self.scan(name+'!/'+i.name,t.extractfile(i).read(),depth+1)
   elif Path(name).suffix.lower() in {'.7z','.rar','.xz','.bz2','.zst'}:self.errors.append({'path':name,'reason':'unsupported container'})
  except Exception as e:self.errors.append({'path':name,'reason':type(e).__name__+': '+str(e)[:120]})
 def tree(self,root):
  for p in sorted(root.rglob('*')):
   rel=p.relative_to(root).as_posix()
   if rel.startswith('publication/audit-results') or p.name=='SHA256SUMS':continue
   if p.is_symlink():self.add(rel,'filesystem_symlink');continue
   if p.is_file():self.scan(rel,p.read_bytes())
def main():
 a=argparse.ArgumentParser();a.add_argument('root',type=Path);a.add_argument('output',type=Path);o=a.parse_args()
 s=Scanner();s.tree(o.root)
 result={'scope':'all regular files; nested ZIP, gzip, tar; raw bytes and nul-stripped strings; no execution','entries':len(s.entries),'expanded_bytes':s.total,'findings':s.findings,'errors':s.errors,'inventory':s.entries}
 o.output.write_text(json.dumps(result,indent=2)+'\n')
 print(json.dumps({'entries':len(s.entries),'expanded_bytes':s.total,'findings':dict(collections.Counter(x['category'] for x in s.findings)),'errors':len(s.errors)}))
if __name__=='__main__':main()
