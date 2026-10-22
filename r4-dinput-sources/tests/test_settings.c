/* SPDX-License-Identifier: LGPL-2.1-or-later */
#include "ps2_settings.h"
#include <assert.h>
#include <stdio.h>
static void fixture(unsigned char *p) {
    memset(p,0,PW_PS2_SIZE); pw_ps2_put(p,6);
    for(unsigned t=0;t<4;t++) for(unsigned a=0;a<59;a++) {
        size_t o=pw_ps2_record(t,a); pw_ps2_put(p+o,a);
        pw_ps2_put(p+o+4,100+t+a); pw_ps2_put(p+o+8,t+1);
    }
    unsigned char *tail=p+PW_PS2_TAIL;
    for(unsigned i=0;i<64;i++) tail[i]=(unsigned char)(i+25);
    tail[26]='T';tail[47]=0x1d;tail[63]='_';tail[38]=0;
    memset(tail+48,1,6);
}
int main(void) {
    unsigned char p[PW_PS2_SIZE],before[PW_PS2_SIZE],once[PW_PS2_SIZE];
    fixture(p);memcpy(before,p,sizeof(p));assert(pw_ps2_apply(p,sizeof(p),0));
    for(unsigned i=0;i<sizeof(p);i++) {
        int changed_control=i>=pw_ps2_record(3,0) && i<PW_PS2_TAIL && ((i-4)%12)>=4;
        int changed_option=i==PW_PS2_TAIL+38 || (i>=PW_PS2_TAIL+48 && i<PW_PS2_TAIL+54);
        if(!changed_control && !changed_option) assert(p[i]==before[i]);
    }
    for(unsigned i=0;i<sizeof(pw_ps2_map)/sizeof(pw_ps2_map[0]);i++)
        assert(pw_ps2_get(p+pw_ps2_record(3,pw_ps2_map[i].action)+4)==pw_ps2_map[i].button);
    assert(pw_ps2_get(p+pw_ps2_record(3,24)+4)==2);
    assert(pw_ps2_get(p+pw_ps2_record(3,25)+4)==3);
    assert(pw_ps2_get(p+pw_ps2_record(3,45)+4)==8);
    memcpy(once,p,sizeof(p));assert(pw_ps2_apply(p,sizeof(p),0));assert(!memcmp(p,once,sizeof(p)));
    assert(pw_ps2_apply(p,sizeof(p),1));assert(p[PW_PS2_TAIL+51]==1);
    for(unsigned i=0;i<sizeof(p);i++) if(i!=PW_PS2_TAIL+51) assert(p[i]==once[i]);
    for(unsigned size=0;size<PW_PS2_SIZE;size++) assert(!pw_ps2_apply(p,size,0));
    assert(!pw_ps2_apply(p,PW_PS2_SIZE+1,0));
    fixture(p);p[0]=5;memcpy(before,p,sizeof(p));assert(!pw_ps2_apply(p,sizeof(p),0));assert(!memcmp(p,before,sizeof(p)));
    fixture(p);p[pw_ps2_record(2,35)]=0;assert(!pw_ps2_apply(p,sizeof(p),0));
    fixture(p);p[PW_PS2_TAIL+26]=0;assert(!pw_ps2_apply(p,sizeof(p),0));
    fixture(p);pw_ps2_put(p,7);p[PW_PS2_TAIL+11]='T';p[PW_PS2_TAIL+32]=0x1d;
    memcpy(before,p,sizeof(p));assert(pw_ps2_apply(p,sizeof(p),1));
    assert(!memcmp(p+PW_PS2_TAIL,before+PW_PS2_TAIL,64));
    fixture(p);pw_ps2_put(p,8);assert(!pw_ps2_apply(p,sizeof(p),0));
    puts("PASS settings: contexts, brake/reverse, analog-only movement, graphics/audio/keyboard bytes preserved, inversion isolated, idempotence, malformed/truncated rejection");
}
