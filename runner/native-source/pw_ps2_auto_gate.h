/* SPDX-License-Identifier: LGPL-2.1-or-later */
#ifndef PW_PS2_AUTO_GATE_H
#define PW_PS2_AUTO_GATE_H
#include <stdint.h>
typedef struct PwPs2AutoGate {
    uint64_t stable_since, quiet_since;
    int stable, quiet, active;
} PwPs2AutoGate;

/* GetDeviceState is also called briefly during startup/loading. Require a
 * continuous stream of fresh records, then a released controller, rather
 * than handing over on the first successful read or a fixed launch delay.
 * This is an input-readiness heuristic, not a game loading-state detector. */
static inline int pw_ps2_auto_gate(PwPs2AutoGate *gate, uint64_t now,
                                   uint64_t progress, int healthy, int neutral)
{
    if (!healthy || now < progress || now-progress > 750000000ull) {
        gate->stable=gate->quiet=gate->active=0;
        return 0;
    }
    if (gate->active) return 1;
    if (!gate->stable) { gate->stable=1; gate->stable_since=now; }
    if (!neutral) gate->quiet=0;
    else if (!gate->quiet) { gate->quiet=1; gate->quiet_since=now; }
    gate->active = now-gate->stable_since >= 4000000000ull && gate->quiet &&
                   now-gate->quiet_since >= 300000000ull;
    return gate->active;
}

static inline int pw_ps2_auto_neutral(uint32_t buttons,unsigned lx,unsigned ly,
                                     unsigned rx,unsigned ry,unsigned l2,unsigned r2)
{
    return !buttons && lx>=104 && lx<=152 && ly>=104 && ly<=152 &&
           rx>=104 && rx<=152 && ry>=104 && ry<=152 && l2<=30 && r2<=30;
}
#endif
