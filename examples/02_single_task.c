#include <stdio.h>

#include <gt/gt.h>

static void
worker_task_entry(void *arg)
{
    const char *message = arg;

    printf("[task] Worker task started.\n");
    printf("[task] Message: \"%s\"\n", message);
}

int
main(void)
{
    printf("[example] Booting Green Threads runtime...\n");

    gt_status_t status = gt_init();
    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Runtime initialization failed: %s\n", gt_status_string(status));
        return 1;
    }

    const char *payload = "Hello from the Green Threads runtime!";

    /*
     * Create a single task.
     *
     * Passing a stack size of zero requests the runtime's default
     * stack size.
     */
    printf("[example] Creating worker task...\n");

    status = gt_create_task(worker_task_entry, (void *)payload, 0);
    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Task creation failed: %s\n", gt_status_string(status));

        gt_destroy();
        return 1;
    }

    printf("[example] Starting scheduler...\n");

    gt_run();

    printf("[example] All tasks completed successfully.\n");

    gt_destroy();

    printf("[example] Runtime shut down successfully.\n");

    return 0;
}
