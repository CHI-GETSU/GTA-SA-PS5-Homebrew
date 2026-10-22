/* SPDX-License-Identifier: LGPL-2.1-or-later
 * Preserve the existing XInput axis contract used by Sahin/Prospero Win.
 * The R2 safety gate still owns buttons and the right-stick mouse path.
 * Include after pw_wine_display.h (PwWinePad / PwWineInput definitions).
 */
#ifndef PW_GTA_HYBRID_H
#define PW_GTA_HYBRID_H
static inline void pw_gta_hybrid_left(const PwWinePad *physical, PwWinePad *sent,
                                      int enabled, int forced_keyboard)
{
    if (enabled && !forced_keyboard) {
        sent->thumb_lx = physical->thumb_lx;
        sent->thumb_ly = physical->thumb_ly;
    }
}
static inline void pw_gta_hybrid_camera(PwWineInput *event, int invert)
{
    if (invert && event->type == PW_WINE_INPUT_MOUSE_MOVE) event->y = -event->y;
}
#endif
