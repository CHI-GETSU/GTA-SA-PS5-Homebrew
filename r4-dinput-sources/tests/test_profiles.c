/* SPDX-License-Identifier: LGPL-2.1-or-later */
#include "pw_game_profile.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static PwGameProfile read_profile(const char *path) {
    unsigned char bytes[8192]; PwGameProfile p;
    FILE *f=fopen(path,"rb");assert(f);size_t n=fread(bytes,1,sizeof(bytes),f);fclose(f);
    assert(pw_game_profile_parse(bytes,n,&p)==0);return p;
}
int main(int argc,char **argv) {
    assert(argc==8);PwGameProfile old=read_profile(argv[1]);
    for(int i=2;i<argc;i++) {
        PwGameProfile p=read_profile(argv[i]);
        assert(!strcmp(p.app.prefix,"gta-sa-ps2-r4"));
        assert(strstr(p.app.dll_overrides,"wmvcore="));
        assert(strstr(p.app.dll_overrides,"dinput8=b"));
        assert(p.debug_env_count<=5);
        if(strstr(p.app.executable,"gta_sa.exe")) {
            assert(!memcmp(&p.input,&old.input,sizeof(p.input)));
            assert(!memcmp(&p.display,&old.display,sizeof(p.display)));
            assert(!memcmp(&p.runtime,&old.runtime,sizeof(p.runtime)));
            assert(p.app.graphics==old.app.graphics);
            if(!strcmp(p.app.id,"gta-sa-ps2-r4")) {
                int enabled=0;
                for(size_t j=0;j<p.debug_env_count;j++)
                    if(!strcmp(p.debug_env[j].name,"PW_DINPUT_GTA_PS2") &&
                       !strcmp(p.debug_env[j].value,"1")) enabled++;
                assert(enabled==1);
            } else assert(p.debug_env_count==old.debug_env_count);
        }
        printf("PASS R4 profile parser and preserved runtime/display/fallback: %s\n",p.app.id);
    }
}
