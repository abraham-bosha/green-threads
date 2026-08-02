#include <stdio.h>

#include <gt/gt.h>

static const size_t STANDARD_STACK_SIZE = 16 * 1024;

static void
child_task_entry(void *arg)
{
    const char *name = arg;

    printf("[child] Started execution.\n");
    printf("[child] Argument: \"%s\"\n", name);
    printf("[child] Completed successfully.\n");
}

static void
parent_task_entry(void *arg)
{
    const char *name = arg;
    const char *child_name = "Child Task";

    printf("[parent] Started execution.\n");
    printf("[parent] Argument: \"%s\"\n", name);

    printf("[parent] Creating a nested task...\n");

    gt_status_t status = gt_create_task(child_task_entry, (void *)child_name, STANDARD_STACK_SIZE);

    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[parent] Failed to create child task: %s\n", gt_status_string(status));
        return;
    }

    printf("[parent] Child task admitted to the ready queue.\n");
    printf("[parent] Parent task finished.\n");
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

    const char *parent_name = "Parent Task";

    printf("[example] Scheduling the parent task...\n");

    status = gt_create_task(parent_task_entry, (void *)parent_name, STANDARD_STACK_SIZE);

    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Failed to create parent task: %s\n", gt_status_string(status));
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
