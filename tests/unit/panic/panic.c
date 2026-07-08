#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <gt_internal/panic/panic.h>

void
test_panic(void)
{
    pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0)
        GT_PANIC("panic test");

    int status;

    assert(waitpid(pid, &status, 0) == pid);

    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
}

int 
main(void)
{
    printf("[RUN] panic/panic\n");

    test_panic();

    printf("[PASSED] panic/panic\n");

    return 0;
}
