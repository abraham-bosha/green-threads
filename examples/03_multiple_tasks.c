#include <stdio.h>

#include <gt/gt.h>

static void
worker_task_entry(void *arg)
{
    const char *task_name = arg;

    printf("[task] %s: started\n", task_name);
    printf("[task] %s: performing work\n", task_name);
    printf("[task] %s: finished\n", task_name);
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

    printf("[example] Scheduling three worker tasks...\n");

    status = gt_create_task(worker_task_entry, "Task A", 0);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(worker_task_entry, "Task B", 0);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(worker_task_entry, "Task C", 0);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    printf("[example] Starting scheduler...\n");

    gt_run();

    printf("[example] All tasks completed successfully.\n");

    gt_destroy();

    printf("[example] Runtime shut down successfully.\n");

    return 0;

cleanup:
    fprintf(stderr, "[error] Failed to create task: %s\n", gt_status_string(status));

    gt_destroy();

    return 1;
}
