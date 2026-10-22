/* SPDX-License-Identifier: LGPL-2.1-or-later
 * Regression probe only: no change to AUTO2 or runtime controls. */
#include "pw_wine_display.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
static PwPadPs5Data sample;
static int mock_read(int32_t handle,PwPadPs5Data *out,int32_t cap) {
    (void)handle;assert(cap>0);*out=sample;return 1;
}
int main(void) {
    const unsigned char xy[][2]={{128,128},{0,128},{255,128},{128,0},{128,255}};
    const int expected[][2]={{0,0},{-32768,0},{32767,0},{0,32767},{0,-32768}};
    PwPadPs5 pad={0};pad.ops.pad_read=mock_read;pad.opened=1;
    for(unsigned i=0;i<5;i++) {
        memset(&sample,0,sizeof(sample));sample.connected=1;sample.connected_count=1;
        sample.timestamp=i+1;sample.left_stick=(PwPadPs5Stick){128,128};
        sample.right_stick=(PwPadPs5Stick){xy[i][0],xy[i][1]};
        assert(pw_pad_ps5_read(&pad)==PW_OK);
        PwWinePad out;assert(pw_wine_game_pad(&pad,&out));
        assert(out.thumb_rx==expected[i][0] && out.thumb_ry==expected[i][1]);
        assert(!out.thumb_lx && !out.thumb_ly && !out.buttons && !out.left_trigger && !out.right_trigger);
        printf("PASS ScePad sample -> XInput right vector %u: %d,%d\n",i,out.thumb_rx,out.thumb_ry);
    }
    puts("PASS existing pad reader/conversion, all right-stick cardinal directions; no production source changed");
}
