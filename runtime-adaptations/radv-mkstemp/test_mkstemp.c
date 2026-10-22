#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

extern int mkstemp(char *);
#ifndef EXPECTED_ENOSYS
#define EXPECTED_ENOSYS ENOSYS
#endif
#ifdef TARGET_OBJECT_TEST
/* Supply only the FreeBSD errno accessor when exercising the PS5 object. */
int *__error(void) { return &errno; }
#endif

int main(void)
{
    char pattern[] = "must-not-create-XXXXXX";
    char original[sizeof(pattern)];
    struct stat st;
    memcpy(original, pattern, sizeof(pattern));
    errno = 0;
    assert(mkstemp(pattern) == -1);
    assert(errno == EXPECTED_ENOSYS);
    assert(memcmp(pattern, original, sizeof(pattern)) == 0);
    assert(lstat(pattern, &st) == -1 && errno == ENOENT);
    /* The refusal path must not even dereference the template. */
    errno = 0;
    assert(mkstemp((char *)0) == -1 && errno == EXPECTED_ENOSYS);
    return 0;
}
