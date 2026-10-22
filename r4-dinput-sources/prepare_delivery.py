from pathlib import Path
import subprocess,shutil,json,hashlib,difflib
root=Path(__file__).resolve().parent; r3=root.parent/'gta-sa-sahin-r3-complet'
out=root.parent/'gta-sa-ps2-r4-complet'; out.mkdir(exist_ok=True)
data=out/'ps5-data/prospero-win'; (data/'prefixes').mkdir(parents=True,exist_ok=True)
profiles=data/'profiles'; profiles.mkdir(exist_ok=True)
prefix=data/'prefixes/gta-sa-ps2-r4'
original=r3/'ps5-data/prospero-win/prefixes/gta-sa-sahin-r3'
if not prefix.exists():
    subprocess.run(['cp','-a','--reflink=auto',str(original),str(prefix)],check=True)
tool=prefix/'drive_c/Tools/PWPadR4'; tool.mkdir(parents=True,exist_ok=True)
shutil.copy2(root/'test-bin/ps2-setup.exe',tool/'ps2-setup.exe')
base=(r3/'ps5-data/prospero-win/profiles/gta-sa-sahin-r3.profile').read_text()
base=base.replace('prefix = gta-sa-sahin-r3','prefix = gta-sa-ps2-r4')
def identity(text,id,name):
    return text.replace('id = gta-sa-sahin-r3','id = '+id).replace('name = GTA SA R3 - hybride Sahin','name = '+name)
safe=identity(base,'gta-sa-ps2-r4-secours','GTA SA R4 - secours R3 conserve')
console=identity(base,'gta-sa-ps2-r4','GTA SA R4 - PS2 natif (R3+tactile)')
console=console.replace('env = PW_DINPUT_XINPUT=1','env = PW_DINPUT_XINPUT=1\nenv = PW_DINPUT_GTA_PS2=1')
console=console.replace('; Hybrid: analog left stick; existing keyboard buttons and right-stick mouse.',
    '; Before handoff: proven R3 fallback. After R3+touchpad and healthy DI: native PS2 actions in GTA.')
setup=identity(base,'gta-sa-ps2-r4-config','GTA SA R4 - 1 configurer PS2')
setup=setup.replace('C:\\Games\\GTASA\\gta_sa.exe','C:\\Tools\\PWPadR4\\ps2-setup.exe').replace(
    'working_directory = C:\\Games\\GTASA','working_directory = C:\\Tools\\PWPadR4')
setup=setup.replace('mode = xinput','mode = keyboard').replace(
    'env = PW_DINPUT_XINPUT=1\nenv = PW_GTA_HYBRID_LEFT=1\nenv = PW_GTA_MOUSE_FLIP_Y=1',
    'env = PW_DINPUT_XINPUT=0').replace('winedebug = -all,+pwpad','winedebug = -all,+pwpad,+debugstr')
camera=setup.replace('id = gta-sa-ps2-r4-config','id = gta-sa-ps2-r4-camera').replace(
    'name = GTA SA R4 - 1 configurer PS2','name = GTA SA R4 - camera Y autre sens').replace(
    'working_directory =','arguments = --right-y-original\nworking_directory =')
restore=setup.replace('id = gta-sa-ps2-r4-config','id = gta-sa-ps2-r4-restaurer').replace(
    'name = GTA SA R4 - 1 configurer PS2','name = GTA SA R4 - restaurer reglages avant PS2').replace(
    'working_directory =','arguments = --restore\nworking_directory =')
probe=(root.parent/'gta-sa-dualsense-recovery-r2/ps5-data/prospero-win/profiles/gta-sa-pad-r2-probe.profile').read_text()
probe=probe.replace('gta-sa-pad-r2-probe','gta-sa-ps2-r4-diagnostic').replace('prefix = gta-sa-pad-r2','prefix = gta-sa-ps2-r4').replace(
    'name = DualSense R2 - diagnostic 20 secondes','name = GTA SA R4 - diagnostic DualSense 20s').replace(
    'env = PW_DINPUT_XINPUT=1','env = PW_DINPUT_XINPUT=1\nenv = PW_DINPUT_GTA_PS2=1')
items=[('gta-sa-ps2-r4-secours',safe),('gta-sa-ps2-r4-config',setup),('gta-sa-ps2-r4',console),
       ('gta-sa-ps2-r4-camera',camera),('gta-sa-ps2-r4-restaurer',restore),('gta-sa-ps2-r4-diagnostic',probe)]
for id,text in items: (profiles/(id+'.profile')).write_text(text)
(profiles/'profiles.lst').write_text(''.join(id+'.profile\n' for id,_ in items))
overlay=out/'runtime-overlay/win/wine/lib/wine/i386-windows'; overlay.mkdir(parents=True,exist_ok=True)
shutil.copy2(root/'wine-build/dlls/dinput8/i386-windows/dinput8.dll',overlay/'dinput8.dll')
src=out/'sources-r4';src.mkdir(exist_ok=True)
for name in ('tools','tests','patches'):
    shutil.copytree(root/name,src/name,dirs_exist_ok=True)
for name in ('prepare_delivery.py','test_host.py'):
    shutil.copy2(root/name,src/name)
shutil.copy2(r3/'PPSA99995/LICENSE',src/'LGPL-2.1.txt')
old=(root.parent/'gta-sa-dualsense-recovery-r2/patches/joystick_xinput.c').read_text()
new=(root/'patches/joystick_xinput.c').read_text()
(src/'r3-to-r4-dinput.patch').write_text(''.join(difflib.unified_diff(old.splitlines(True),new.splitlines(True),
    fromfile='a/dlls/dinput/joystick_xinput.c',tofile='b/dlls/dinput/joystick_xinput.c')))
print(json.dumps(dict(delivery=str(out),profiles=len(items),game=str(prefix/'drive_c/Games/GTASA'),
    eboot_changed=False,runtime_changed=['win/wine/lib/wine/i386-windows/dinput8.dll']),indent=2))
