/* SPDX-License-Identifier: LGPL-2.1-or-later */
#ifndef PW_PAD_RECOVERY_H
#define PW_PAD_RECOVERY_H
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* A fresh heartbeat proves that this session's DirectInput client completed
 * GetDeviceState successfully. It does not prove the game's control mode. */
typedef struct PwPadRecovery {
    unsigned long long sequence;
    uint64_t last_progress;
    int healthy;
} PwPadRecovery;

static inline int pw_pad_recovery_keys(int analog_requested, int forced_keys, int healthy)
{
    return forced_keys || !analog_requested || !healthy;
}

static inline void pw_pad_recovery_buttons(uint32_t physical, int keys, uint32_t *previous,
                                           uint32_t *pressed, uint32_t *released)
{
    uint32_t next = keys ? physical : 0;
    *pressed = next & ~*previous;
    *released = *previous & ~next;
    *previous = next;
}

static inline int pw_pad_recovery_step(PwPadRecovery *state, const char *session,
                                      const char *record, uint64_t now)
{
    char received[64] = "", tail;
    unsigned long long sequence = 0;
    int valid = record && sscanf(record, "PWDI2 %63s %llu %c", received, &sequence, &tail) == 2 &&
                !strcmp(session, received) && sequence != 0;
    if (valid && sequence > state->sequence) {
        state->sequence = sequence;
        state->last_progress = now;
        state->healthy = 1;
    }
    if (state->healthy && now - state->last_progress > 1500000000ull) state->healthy = 0;
    return state->healthy;
}
#endif
