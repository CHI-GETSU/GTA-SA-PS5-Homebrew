from pathlib import Path
import subprocess,shutil,json,hashlib,struct,re
root=Path(__file__).resolve().parent; out=root.parent/'gta-sa-ps2-r4-complet'
r3=root.parent/'gta-sa-sahin-r3-complet'; original=r3/'ps5-data/prospero-win/prefixes/gta-sa-sahin-r3'
prefix=out/'ps5-data/prospero-win/prefixes/gta-sa-ps2-r4'
def sha(p):
    h=hashlib.sha256()
    with p.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''):h.update(b)
    return h.hexdigest()
expected=json.loads((r3/'verification-prefixe.json').read_text())['files']
for name,entry in expected.items():
    a=original/name;b=prefix/name
    assert a.lstat().st_ino!=b.lstat().st_ino,name
    if 'symlink' in entry:
        assert str(a.readlink())==str(b.readlink())==entry['symlink']
    else:
        assert sha(a)==sha(b)==entry['sha256'],name
extra={str(p.relative_to(prefix)) for p in prefix.rglob('*') if p.is_file() or p.is_symlink()}-expected.keys()
assert extra=={'drive_c/Tools/PWPadR4/ps2-setup.exe'},extra
assert sha(r3/'PPSA99995/eboot.bin')=='b137f76bd7fbc200ce931b439c9fe741a38d6ca51c819c3a2f516535a9be8a44'
old_dll=r3/'PPSA99995/win/wine/lib/wine/i386-windows/dinput8.dll'
assert sha(old_dll)=='dca5fd3739878eea55aaef6b62af9f7433a666835a58bdcccc2285bc4e925db5'
dll=out/'runtime-overlay/win/wine/lib/wine/i386-windows/dinput8.dll'
assert sha(dll)==sha(root/'wine-build/dlls/dinput8/i386-windows/dinput8.dll')
for p in (dll,prefix/'drive_c/Tools/PWPadR4/ps2-setup.exe'):
    data=p.read_bytes();pe=struct.unpack_from('<I',data,60)[0]
    assert data[pe:pe+4]==b'PE\0\0' and struct.unpack_from('<H',data,pe+4)[0]==0x14c
    assert struct.unpack_from('<H',data,pe+24)[0]==0x10b
def imports(p):
    return sorted(re.findall(r'DLL Name: (.+)',subprocess.check_output(['objdump','-p',str(p)],text=True)))
assert imports(dll)==imports(old_dll)
source=root.parent/'v0.2.0/src'; exe=root/'test-bin/test-profiles'
subprocess.run(['cc','-std=c11','-O2','-Wall','-Wextra','-Werror','-I'+str(source),
    str(root/'tests/test_profiles.c'),str(source/'pw_game_profile.c'),str(source/'pw_app_profile.c'),'-o',str(exe)],check=True)
profiles=sorted((out/'ps5-data/prospero-win/profiles').glob('*.profile'))
old_profile=r3/'ps5-data/prospero-win/profiles/gta-sa-sahin-r3.profile'
profile_result=subprocess.check_output([str(exe),str(old_profile),*map(str,profiles)],text=True)
core_result=subprocess.check_output([str(root/'test-bin/test-settings')],text=True)
src=out/'sources-r4'
shutil.copy2(root/'RECONSTRUCTION.md',src/'RECONSTRUCTION.md')
shutil.copy2(Path(__file__),src/'verify_delivery.py')
for name in ('dinput','dinput8'):
    shutil.copytree(root/'wine-source/dlls'/name,src/'wine-dinput/dlls'/name,dirs_exist_ok=True)
evidence=out/'tests-locaux';evidence.mkdir(exist_ok=True)
for name in ('host-tests.json',):shutil.copy2(root/name,evidence/name)
for p in (root/'logs').glob('*.log'):shutil.copy2(p,evidence/p.name)
result=dict(ps5_validated=False,transferred=False,
    base='R3',runtime_changes=['win/wine/lib/wine/i386-windows/dinput8.dll'],
    eboot_changed=False,graphics_changed=False,imports_identical=imports(dll),
    original_r3_prefix_unchanged=True,prefix_inherited_files_verified=len(expected),
    additional_prefix_files=sorted(extra),shared_file_inodes=False,
    dinput8_sha256=sha(dll),setup_sha256=sha(prefix/'drive_c/Tools/PWPadR4/ps2-setup.exe'),
    core_tests=core_result.strip(),profile_tests=profile_result.strip(),
    reference_files={p.name:sha(p) for p in sorted((root/'reference').iterdir()) if p.is_file()},
    host_tests=json.loads((root/'host-tests.json').read_text()))
(out/'VERIFICATION.json').write_text(json.dumps(result,indent=2)+'\n')
manifest={str(p.relative_to(out)):dict(bytes=p.stat().st_size,sha256=sha(p))
          for p in sorted(out.rglob('*')) if p.is_file() and p.name!='INSTALL_MANIFEST.json'}
(out/'INSTALL_MANIFEST.json').write_text(json.dumps(manifest,indent=2)+'\n')
print(json.dumps({k:v for k,v in result.items() if k not in ('reference_files','host_tests','profile_tests')},indent=2))
print(profile_result)
