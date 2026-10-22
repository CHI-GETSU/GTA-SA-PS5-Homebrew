/* SPDX-License-Identifier: LGPL-2.1-or-later
 * Local link adaptation, based on wine/ps5/pw_vulkan_libc.c at v0.2.0.
 * PS5 titles cannot run CLRX. Refuse its optional temporary-file step.
 * Hidden visibility prevents this implementation becoming a module export.
 */
#include <errno.h>

__attribute__((visibility("hidden"))) int mkstemp(char *pattern)
{
    (void)pattern;
    errno = ENOSYS;
    return -1;
}
