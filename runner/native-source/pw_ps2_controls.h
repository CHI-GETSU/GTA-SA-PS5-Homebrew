/* SPDX-License-Identifier: LGPL-2.1-or-later */
#ifndef PW_PS2_CONTROLS_H
#define PW_PS2_CONTROLS_H
#include <stdint.h>

/* Options remains a keyboard Escape in both input modes. Other bindings
 * are released on handover, so movement/fire never receive duplicate keys. */
static inline uint32_t pw_ps2_keyboard_buttons(uint32_t physical, int keys, int ps2)
{
    return keys ? physical : (ps2 ? physical & 0x8u : 0);
}

/* A bare touchpad click must not tear down the working PS2 controller. */
static inline int pw_ps2_accept_touch(uint32_t physical, int ps2)
{
    return !ps2 || (physical & 0x4u);
}
/* Request native input automatically only for the PS2 game profile.
 * The existing fresh-session heartbeat still decides the actual handover. */
static inline int pw_ps2_auto_request(int bridge, int ps2, int probe)
{
    return bridge && ps2 && !probe;
}
#endif
