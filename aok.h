#ifndef AOK_H
#define AOK_H

#include <stdio.h>
#include <stdlib.h>

static
void
assert_ok(long rv, const char* file, int line)
{
    if (rv == -1) {
        fprintf(stderr, "At %s:%d, ", file, line);
        perror("syscall failed");
        fflush(stderr);
        exit(1);
    }
}

#define aok(rv) assert_ok((long) rv, __FILE__, __LINE__)

#endif
