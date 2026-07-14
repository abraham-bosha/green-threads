#include <gt_internal/panic/panic.h>

#include <stdio.h>

#include <subprocess.h>

void
test_panic(void)
{
    GT_PANIC("panic test");
}

int
main(void)
{
    puts("[RUN] panic");

    verify_process_aborts(test_panic);

    puts("[PASS] panic");

    return 0;
}
