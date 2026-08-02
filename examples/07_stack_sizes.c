#include <stdio.h>

#include <gt/gt.h>

static void
stack_size_task_entry(void *arg)
{
    int stack_size_kib = *(int *)arg;

    printf("[task] Running on a %d KiB stack.\n", stack_size_kib);
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

    const int stack_4k = 4;
    const int stack_8k = 8;
    const int stack_16k = 16;
    const int stack_64k = 64;

    printf("[example] Creating tasks with custom stack sizes...\n");

    status = gt_create_task(stack_size_task_entry, (void *)&stack_4k, 4 * 1024);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(stack_size_task_entry, (void *)&stack_8k, 8 * 1024);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(stack_size_task_entry, (void *)&stack_16k, 16 * 1024);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(stack_size_task_entry, (void *)&stack_64k, 64 * 1024);
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
