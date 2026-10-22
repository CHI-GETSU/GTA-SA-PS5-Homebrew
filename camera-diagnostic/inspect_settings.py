#!/usr/bin/env python3
# SPDX-License-Identifier: LGPL-2.1-or-later
"""Read-only GTA settings inspection. Never rewrites option bytes or saves."""
from pathlib import Path
import argparse,hashlib,json,struct
p=argparse.ArgumentParser();p.add_argument('file',type=Path);o=p.parse_args()
b=o.file.read_bytes();result={'bytes':len(b),'sha256':hashlib.sha256(b).hexdigest(),'version':int.from_bytes(b[:4],'little') if len(b)>=4 else None,'writes_performed':False}
valid=len(b)==2900
if valid:
 for t in range(4):
  for a in range(59):
   off=4+(t*59+a)*12
   if struct.unpack_from('<I',b,off)[0]!=a or struct.unpack_from('<I',b,off+8)[0]>4:valid=False
result['action_table_structure_valid']=valid
if valid:
 tail=b[2836:];version=result['version'];result['known_markers']={'v6':tail[26]==84 and tail[47]==29 and tail[63]==95,'v7':tail[11]==84 and tail[32]==29 and tail[63]==95}
 if version==6 and result['known_markers']['v6']:
  result['v6_control_method_byte']=tail[38]
  result['control_mode_interpretation']='Legacy v6 helper offset only; verify the controller mode in the game menu'
 else:result['control_mode_interpretation']='Not decoded: do not infer v7 offsets from v6. Verify Joypad in the game menu.'
print(json.dumps(result,indent=2))
