#include <assert.h>
#include <signal.h>
#include <stdlib.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define OUTPUT_BUFFER_SIZE 4096

static ssize_t
read_all(int fd, char *buffer, size_t size);

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

void
verify_process_output(void (*func)(void), void (*verify)(const char *output))
{
    char output[OUTPUT_BUFFER_SIZE];
    int pipefds[2];
    __attribute__((unused)) int status;
    ssize_t bytes_read;
    __attribute__((unused)) int result;

    result = pipe(pipefds);
    assert(result == 0);

    pid_t pid = fork();
    assert(pid >= 0);

    if (pid == 0)
    {
        close(pipefds[0]);

        result = dup2(pipefds[1], STDERR_FILENO);
        assert(result >= 0);

        result = dup2(pipefds[1], STDOUT_FILENO);
        assert(result >= 0);

        close(pipefds[1]);

        func();

        exit(EXIT_SUCCESS);
    }

    close(pipefds[1]);
    assert(waitpid(pid, &status, 0) == pid);

    bytes_read = read_all(pipefds[0], output, sizeof(output));

    assert(bytes_read >= 0);
    output[bytes_read] = '\0';

    assert(WIFEXITED(status));
    assert(WEXITSTATUS(status) == EXIT_SUCCESS);

    close(pipefds[0]);

    verify(output);
}

static ssize_t
read_all(int fd, char *buffer, size_t size)
{
    ssize_t bytes_read;
    size_t total_bytes;

    total_bytes = 0;

    while (total_bytes < size - 1 &&
           (bytes_read = read(fd, buffer + total_bytes, size - 1 - total_bytes)) > 0)
    {
        total_bytes += (size_t)bytes_read;
    }

    if (bytes_read < 0)
        return -1;

    return (ssize_t)total_bytes;
}
