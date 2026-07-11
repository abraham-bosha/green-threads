#include <assert.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void
verify_process_aborts(void (*func)(void))
{
    pid_t pid = fork();

    assert(pid >= 0);

    if (pid == 0)
    {
        func();

        /* Should never reach here. */
        _exit(EXIT_FAILURE);
    }

    int status;

    __attribute__((unused)) pid_t reaped_pid = waitpid(pid, &status, 0);

    assert(reaped_pid == pid);

    assert(WIFSIGNALED(status));
    assert(WTERMSIG(status) == SIGABRT);
}
