#include <stdint.h>
#include <stdio.h>

#include <gt/gt.h>

struct fibonacci_task
{
    int index;
    uint64_t result;
};

static void
fib_task_entry(void *arg)
{
    struct fibonacci_task *task = arg;
    if (task == NULL)
        return;

    const int n = task->index;

    printf("[task %2d] Computing F(%d)...\n", n, n);

    if (n <= 0)
    {
        task->result = 0;
    } else if (n == 1)
    {
        task->result = 1;
    } else
    {
        uint64_t a = 0;
        uint64_t b = 1;

        for (int i = 2; i <= n; ++i)
        {
            const uint64_t next = a + b;
            a = b;
            b = next;

            /*
             * Yield periodically to allow other tasks to execute.
             * This demonstrates cooperative multitasking.
             */
            if ((i % 5) == 0)
            {
                gt_yield();
            }
        }

        task->result = b;
    }

    printf("[task %2d] Result = %llu\n", n, (unsigned long long)task->result);
}

int
main(void)
{
    printf("[example] Initializing Green Threads Runtime...\n");

    gt_status_t status = gt_init();
    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Runtime initialization failed: %s\n", gt_status_string(status));
        return 1;
    }

    const size_t stack_size = 16 * 1024;

    struct fibonacci_task tasks[] = {
        {.index = 5, .result = 0},
        {.index = 10, .result = 0},
        {.index = 20, .result = 0},
        {.index = 30, .result = 0},
        {.index = 40, .result = 0},
    };

    const size_t task_count = sizeof(tasks) / sizeof(tasks[0]);

    printf("[example] Scheduling %zu Fibonacci tasks...\n", task_count);

    for (size_t i = 0; i < task_count; ++i)
    {
        status = gt_create_task(fib_task_entry, &tasks[i], stack_size);
        if (status != GT_STATUS_SUCCESS)
            goto cleanup;
    }

    printf("[example] Starting scheduler...\n");
    gt_run();

    printf("\nResults:\n");

    for (size_t i = 0; i < task_count; ++i)
    {
        printf("  F(%2d) = %llu\n", tasks[i].index, (unsigned long long)tasks[i].result);
    }

    gt_destroy();

    printf("\n[example] Runtime shut down successfully.\n");
    return 0;

cleanup:
    fprintf(stderr, "[error] Failed to create task: %s\n", gt_status_string(status));

    gt_destroy();
    return 1;
}
