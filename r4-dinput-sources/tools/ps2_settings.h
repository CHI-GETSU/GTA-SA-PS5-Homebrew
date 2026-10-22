/* SPDX-License-Identifier: LGPL-2.1-or-later
 * Independent implementation of the documented GTA PC v6 settings layout.
 * No game code, addresses, hooks, patches or graphics defaults are embedded.
 */
#ifndef PW_PS2_SETTINGS_H
#define PW_PS2_SETTINGS_H
#include <stddef.h>
#include <stdint.h>
#include <string.h>
enum { PW_PS2_ACTIONS=59, PW_PS2_TYPES=4, PW_PS2_TAIL=4+59*4*12,
       PW_PS2_SIZE=PW_PS2_TAIL+64 };
typedef struct PwPs2Map { unsigned action, button; } PwPs2Map;
static const PwPs2Map pw_ps2_map[]={
    {0,1},{1,7},{2,6},{3,5},{8,5},{9,6},{10,4},{11,9},
    {12,3},{13,2},{14,11},{15,10},{16,7},{18,1},{19,7},
    {24,2},{25,3},{26,13},{27,15},{28,14},{29,10},{30,11},
    {31,8},{34,5},{35,6},{44,7},{45,8},{47,14},{48,16},{49,13},{50,15}
};
static uint32_t pw_ps2_get(const unsigned char *p) {
    return (uint32_t)p[0]|(uint32_t)p[1]<<8|(uint32_t)p[2]<<16|(uint32_t)p[3]<<24;
}
static void pw_ps2_put(unsigned char *p,uint32_t v) {
    for(unsigned i=0;i<4;i++) p[i]=(unsigned char)(v>>(8*i));
}
static size_t pw_ps2_record(unsigned type,unsigned action) { return 4+(type*59+action)*12; }
static int pw_ps2_validate(const unsigned char *p,size_t size) {
    if(size!=PW_PS2_SIZE || (pw_ps2_get(p)!=6 && pw_ps2_get(p)!=7)) return 0;
    for(unsigned t=0;t<4;t++) for(unsigned a=0;a<59;a++) {
        size_t o=pw_ps2_record(t,a);
        if(pw_ps2_get(p+o)!=a || pw_ps2_get(p+o+8)>4) return 0;
    }
    const unsigned char *tail=p+PW_PS2_TAIL;
    /* Actual PS5-exported v7 file: same action tables, different option layout.
     * Recognise its markers, but never write or infer its option offsets. */
    if(pw_ps2_get(p)==7) return tail[11]=='T' && tail[32]==0x1d && tail[63]=='_';
    if(tail[26]!='T' || tail[47]!=0x1d || tail[63]!='_' || tail[38]>1) return 0;
    for(unsigned i=48;i<54;i++) if(tail[i]>1) return 0;
    return 1;
}
static int pw_ps2_apply(unsigned char *p,size_t size,int invert_right_y) {
    if(!pw_ps2_validate(p,size)) return 0;
    /* Replace only joystick mappings; preserve both keyboard maps and mouse. */
    for(unsigned a=0;a<59;a++) {
        size_t o=pw_ps2_record(3,a); pw_ps2_put(p+o+4,0); pw_ps2_put(p+o+8,0);
    }
    for(unsigned i=0;i<sizeof(pw_ps2_map)/sizeof(pw_ps2_map[0]);i++) {
        unsigned a=pw_ps2_map[i].action, used=0, order;
        for(unsigned t=0;t<3;t++) used|=1u<<pw_ps2_get(p+pw_ps2_record(t,a)+8);
        for(order=1;order<4 && (used&(1u<<order));order++) {}
        size_t o=pw_ps2_record(3,a);
        pw_ps2_put(p+o+4,pw_ps2_map[i].button); pw_ps2_put(p+o+8,order);
    }
    if(pw_ps2_get(p)==7) return 1; /* Joypad/camera selected in GTA's menu. */
    unsigned char *tail=p+PW_PS2_TAIL;
    tail[38]=1; /* GTA's own Joypad mode, not Prospero's XInput mode. */
    memset(tail+48,0,6); /* No stick-axis swap or left inversion. */
    tail[51]=(unsigned char)!!invert_right_y;
    return 1;
}
#endif
