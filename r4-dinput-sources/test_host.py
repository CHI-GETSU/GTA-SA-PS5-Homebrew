from pathlib import Path
import os,shutil,subprocess,json,struct,sys
root=Path(__file__).resolve().parent
prefix=root/'host-test-prefix'
assert prefix.exists(), 'Use the existing isolated R4 test prefix'
assert (root/'test-bin/title-sink.so').is_file()
assert (root/'wine-build/dlls/xinput1_3/i386-windows/xinput1_3.dll').is_file()
env=dict(os.environ,WINEPREFIX=str(prefix),WINEARCH='win64',USER='prospero',LOGNAME='prospero',
    WINEDLLOVERRIDES='winemenubuilder.exe=d;dinput8=b;xinput1_3=b',WINEDEBUG='-all,+pwpad,+debugstr,+loaddll',
    PW_DINPUT_SESSION='host-r2-test',PW_TEST_PAD_FILE=str(prefix/'drive_c/pw-test-pad.bin'),
    XDG_CACHE_HOME=str(root/'cache'),XDG_RUNTIME_DIR=str(root/'runtime'),TMPDIR=str(root/'tmp'))
wine=root/'wine-build/wine'; server=root/'wine-build/server/wineserver'; results=[]
setup_only='--setup-only' in sys.argv
if setup_only: results=json.loads((root/'host-tests.json').read_text())[:4]
def run(name,exe,args,expected=0):
    subprocess.run([str(server),'-k'],env=env,timeout=10)
    with (root/'logs'/f'{name}.log').open('w') as f:
        p=subprocess.run([str(wine),str(exe),*args],env=env,cwd=root/'test-bin',
                         stdout=f,stderr=f,timeout=50)
    text=(root/'logs'/f'{name}.log').read_text(errors='replace')
    result=dict(test=name,exit_code=p.returncode,expected=expected,
                summary=[s for s in text.splitlines() if s.startswith(('PASS','FAIL','PW_PS2_SETUP'))])
    results.append(result); (root/'host-tests.json').write_text(json.dumps(results,indent=2)+'\n')
    print(json.dumps(result),flush=True)
    assert p.returncode==expected,result
for mode in (() if setup_only else ('disabled','r3-compatible','ps2','missing-backend')):
    env['PW_DINPUT_XINPUT']='0' if mode=='disabled' else '1'
    env.pop('PW_DINPUT_GTA_PS2',None)
    if mode=='ps2': env['PW_DINPUT_GTA_PS2']='1'
    if mode=='missing-backend': env.pop('LD_PRELOAD',None)
    else: env['LD_PRELOAD']=str(root/'test-bin/title-sink.so')
    health=prefix/'drive_c/pw-pad-r2.health'; health.unlink(missing_ok=True)
    run(mode,root/'test-bin/real-xinput-probe.exe',
        [mode if mode in ('disabled','missing-backend') else 'enabled'])
    if mode in ('disabled','missing-backend'): assert not health.exists()
env.pop('LD_PRELOAD',None); env['PW_DINPUT_XINPUT']='0'
folder=prefix/'drive_c/r4-test'; folder.mkdir(exist_ok=True)
def fixture():
    data=bytearray(2900);struct.pack_into('<I',data,0,6)
    for t in range(4):
        for a in range(59):struct.pack_into('<III',data,4+(t*59+a)*12,a,100+t+a,t+1)
    tail=2836; data[tail:]=bytes(range(25,89))
    data[tail+26]=ord('T');data[tail+47]=0x1d;data[tail+63]=ord('_');data[tail+38]=0
    data[tail+48:tail+54]=b'\1'*6
    return bytes(data)
f=folder/'supported.set'; original=fixture(); f.write_bytes(original)
backup=Path(str(f)+'.pw-before-ps2-r4');backup.unlink(missing_ok=True)
args=['--quiet','--file','C:\\r4-test\\supported.set']
run('setup-apply',root/'test-bin/ps2-setup.exe',args)
assert backup.read_bytes()==original
changed=f.read_bytes();assert changed[2836+38]==1
allowed=set()
for a in range(59):allowed.update(range(4+(3*59+a)*12+4,4+(3*59+a)*12+12))
allowed|={2836+38,*range(2836+48,2836+54)}
assert all(a==b for i,(a,b) in enumerate(zip(original,changed)) if i not in allowed)
run('setup-repeat',root/'test-bin/ps2-setup.exe',args)
assert f.read_bytes()==changed and backup.read_bytes()==original
run('setup-restore',root/'test-bin/ps2-setup.exe',args+['--restore'])
assert f.read_bytes()==original
bad=folder/'invalid.set';bad.write_bytes(b'unknown format')
run('setup-reject-format',root/'test-bin/ps2-setup.exe',
    ['--quiet','--file','C:\\r4-test\\invalid.set'],16)
assert bad.read_bytes()==b'unknown format' and not Path(str(bad)+'.pw-before-ps2-r4').exists()
run('setup-missing',root/'test-bin/ps2-setup.exe',
    ['--quiet','--file','C:\\r4-test\\absent.set'],15)
documents=prefix/'drive_c/users/prospero/Documents/GTA San Andreas User Files'
documents.mkdir(parents=True,exist_ok=True)
default=documents/'gta_sa.set'; default.write_bytes(original)
run('setup-default-documents',root/'test-bin/ps2-setup.exe',['--quiet'])
assert default.read_bytes()[2836+38]==1
assert Path(str(default)+'.pw-before-ps2-r4').read_bytes()==original
camera_before=default.read_bytes()
run('setup-camera-original',root/'test-bin/ps2-setup.exe',['--quiet','--right-y-original'])
camera_after=default.read_bytes()
assert camera_after[2836+51]==0
assert all(a==b for i,(a,b) in enumerate(zip(camera_before,camera_after)) if i!=2836+51)
run('setup-camera-alternative',root/'test-bin/ps2-setup.exe',['--quiet','--invert-right-y'])
assert default.read_bytes()[2836+51]==1
run('setup-default-restore',root/'test-bin/ps2-setup.exe',['--quiet','--restore'])
assert default.read_bytes()==original
real_source=Path('${HOME}/Bureau/r3/gta-sa-sahin-r3/drive_c/users/prospero/Documents/GTA San Andreas User Files/gta_sa.set')
real=real_source.read_bytes(); assert len(real)==2900 and struct.unpack_from('<I',real)[0]==7
real_copy=folder/'ps5-v7.set'; real_copy.write_bytes(real)
real_backup=Path(str(real_copy)+'.pw-before-ps2-r4');real_backup.unlink(missing_ok=True)
real_args=['--quiet','--file','C:\\r4-test\\ps5-v7.set']
run('real-ps5-v7-apply',root/'test-bin/ps2-setup.exe',real_args)
patched=real_copy.read_bytes()
assert patched[:4+3*59*12]==real[:4+3*59*12] and patched[2836:]==real[2836:]
for a in range(59):
    o=4+(3*59+a)*12;assert patched[o:o+4]==real[o:o+4]
assert struct.unpack_from('<I',patched,4+(3*59+24)*12+4)[0]==2
assert struct.unpack_from('<I',patched,4+(3*59+25)*12+4)[0]==3
assert real_backup.read_bytes()==real
run('real-ps5-v7-repeat',root/'test-bin/ps2-setup.exe',real_args)
assert real_copy.read_bytes()==patched and real_backup.read_bytes()==real
run('real-ps5-v7-camera-protected',root/'test-bin/ps2-setup.exe',real_args+['--right-y-original'],20)
assert real_copy.read_bytes()==patched
run('real-ps5-v7-restore',root/'test-bin/ps2-setup.exe',real_args+['--restore'])
assert real_copy.read_bytes()==real and real_source.read_bytes()==real
subprocess.run([str(server),'-k'],env=env,timeout=10)
print('PASS host tests: real PE32 DirectInput/XInput/WoW64 and Win32 setup backup/restore; not PS5',flush=True)
