#include <subprocess.h>

#include <gt_internal/panic/panic.h>

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void
test_panic(void)
{
    GT_PANIC("panic test");
}

int
main(void)
{
    puts("[RUN] panic/panic");

    verify_process_aborts(test_panic);

    puts("[PASS] panic/panic");

    return 0;
}
