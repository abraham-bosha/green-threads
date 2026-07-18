#include <gt_internal/panic/panic.h>

#include <stdio.h>
#include <stdlib.h>

GT_NORETURN void
gt_panic(const char *file, int line, const char *function, const char *message)
{
    fprintf(stderr,
            "   | ========================================\n"
            "   | " GT_PANIC_BANNER
            "\n"
            "   | ========================================\n");

    fprintf(stderr, "   | Reason   : %s\n", message ? message : "<no panic message>");

    fprintf(stderr, "   | File     : %s\n", file ? file : "<unknown>");

    fprintf(stderr, "   | Function : %s\n", function ? function : "<unknown>");

    fprintf(stderr, "   | Line     : %d\n", line);

    fprintf(stderr, "   | Aborting...\n\n");

    fflush(stderr);

    abort();
}
