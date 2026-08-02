#include <stdio.h>

#include <gt/gt.h>

static void
task_a_entry(void *arg)
{
    (void)arg;
    printf("[task A] Phase 1: started\n");

    printf("[task A] Yielding...\n");
    gt_yield();

    printf("[task A] Phase 2: resumed\n");
    printf("[task A] Finished\n");
}

static void
task_b_entry(void *arg)
{
    (void)arg;
    printf("[task B] Phase 1: started\n");

    printf("[task B] Yielding...\n");
    gt_yield();

    printf("[task B] Phase 2: resumed\n");
    printf("[task B] Finished\n");
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

    printf("[example] Scheduling two cooperative tasks...\n");

    status = gt_create_task(task_a_entry, NULL, stack_size);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(task_b_entry, NULL, stack_size);
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
