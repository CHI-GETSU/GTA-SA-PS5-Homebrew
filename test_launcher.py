from pathlib import Path
import subprocess,os
root=Path(__file__).resolve().parent; launcher=root/'sahin/launcher'; build=root/'host-tests'; build.mkdir(exist_ok=True)
flags=['-O1','-g','-fsanitize=address,undefined','-fno-sanitize-recover=all','-ffunction-sections','-fdata-sections','-Wno-error=uninitialized-const-pointer']
objects=[]
for name in ('miniz/miniz','miniz/miniz_tinfl','miniz/miniz_tdef','miniz/miniz_zip','yyjson/yyjson'):
    obj=build/(name.replace('/','_')+'.o'); objects.append(str(obj))
    subprocess.run(['/usr/bin/clang','-std=c11',*flags,'-w','-c',str(launcher/'src/third_party'/f'{name}.c'),'-o',str(obj)],check=True)
sources=['library/adopt_gta','library/prefix','library/install_plan','library/game_profile',
         'library/game_install','library/upgrade','library/rtpatch','library/wine_registry',
         'library/base_game','library/tree','core/save_file','core/i18n','core/i18n_strings']
subprocess.run(['/usr/bin/clang++','-std=c++20',*flags,'-Wall','-Wextra','-Werror','-I'+str(launcher/'src'),
    str(root/'test_gta_launcher.cpp'),*[str(launcher/'src'/f'{s}.cpp') for s in sources],*objects,
    '-Wl,--gc-sections','-o',str(build/'gta-launcher-test')],check=True)
environment=dict(os.environ,ASAN_OPTIONS='detect_leaks=1',UBSAN_OPTIONS='halt_on_error=1')
subprocess.run([str(build/'gta-launcher-test'),str(launcher/'assets/plans/GTASA.json'),str(build/'gta-sa-homebrew.profile')],env=environment,check=True)
for name,sources in [('profile',['library/game_profile','core/save_file']),('early_exit',['app/early_exit'])]:
    source='store_game_profile_test.cpp' if name=='profile' else 'store_early_exit_test.cpp'
    binary=build/(name+'-test')
    subprocess.run(['/usr/bin/clang++','-std=c++20',*flags,'-I'+str(launcher/'src'),str(launcher/'tests'/source),
        *[str(launcher/'src'/f'{s}.cpp') for s in sources],'-Wl,--gc-sections','-o',str(binary)],check=True)
    subprocess.run([str(binary)],env=environment,check=True)
