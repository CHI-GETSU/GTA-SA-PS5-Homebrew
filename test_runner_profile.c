#include "pw_game_profile.h"
#include "pw_wine_display.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
int main(int argc,char **argv) {
    assert(argc==2); unsigned char bytes[8192]; PwGameProfile p;
    FILE *file=fopen(argv[1],"rb"); assert(file); size_t n=fread(bytes,1,sizeof(bytes),file); fclose(file);
    assert(pw_game_profile_parse(bytes,n,&p)==0);
    assert(!strcmp(p.app.id,"gta-sa-homebrew"));
    assert(!strcmp(p.app.prefix,"gta-sa-ps2-r4"));
    assert(!strcmp(p.winedebug,"-all,+pwpad"));
    assert(strstr(p.app.dll_overrides,"wmvcore="));
    assert(p.debug_env_count==4 && p.input.mouse==PW_GAME_STICK_RIGHT);
    PwWineInput events[4];
    assert(pw_wine_game_inputs(&p.input,0x8,0,events,4)==1);
    assert(events[0].code==0x1b && events[0].down);
    assert(pw_wine_game_inputs(&p.input,0,0x8,events,4)==1 && !events[0].down);
    puts("PASS generated homebrew profile consumed by actual R4 runner parser and Options event path");
}
