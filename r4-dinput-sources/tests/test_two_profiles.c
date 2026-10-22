/* SPDX-License-Identifier: LGPL-2.1-or-later */
#include "pw_game_profile.h"
#include "pw_wine_display.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static unsigned binding(const PwGameProfile *p,unsigned mask) {
    for(unsigned i=0;i<PW_GAME_BUTTON_COUNT;i++)
        if(p->input.bindings[i].mask==mask) return p->input.bindings[i].code;
    return 0;
}
int main(int argc,char **argv) {
    assert(argc==3);
    for(int a=1;a<argc;a++) {
        unsigned char bytes[8192];PwGameProfile p;FILE *f=fopen(argv[a],"rb");assert(f);
        size_t n=fread(bytes,1,sizeof(bytes),f);fclose(f);
        assert(pw_game_profile_parse(bytes,n,&p)==0);
        assert(!strcmp(p.app.prefix,"gta-sa-ps2-r4"));
        assert(strstr(p.app.dll_overrides,"wmvcore="));assert(strstr(p.app.dll_overrides,"dinput8=b"));
        assert(binding(&p,0x800)=='W');assert(binding(&p,0x100)=='S');
        assert(binding(&p,0x4000)==0x0d);assert(binding(&p,0x2000)==0x11);assert(binding(&p,0x8)==0x1b);
        assert(p.input.mouse==PW_GAME_STICK_RIGHT && p.input.mouse_speed==900);
        PwWineInput event[4];
        assert(pw_wine_game_inputs(&p.input,0x100,0,event,4)==1);
        assert(event[0].type==PW_WINE_INPUT_KEY && event[0].code=='S' && event[0].down==1);
        assert(pw_wine_game_inputs(&p.input,0,0x100,event,4)==1);
        assert(event[0].code=='S' && event[0].down==0);
        assert(pw_wine_game_inputs(&p.input,0x800,0,event,4)==1);
        assert(event[0].code=='W' && event[0].down==1);
        printf("PASS two-profile correction: %s fallback R1=W L2=S; native input retained\n",p.app.id);
    }
}
