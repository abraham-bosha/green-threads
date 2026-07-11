#include <gt_internal/assert/assert.h>

GT_NORETURN void
gt_assertion_failed(const char *exp, const char *file, int line, const char *func, const char *msg)
{
    if (msg == NULL)
        gt_panic(file, line, func, exp);

    gt_panic(file, line, func, msg);
}
