#include <stdio.h>

#include <gt/gt.h>

static const size_t STANDARD_STACK_SIZE = 16 * 1024;

static void
worker_task_entry(void *arg)
{
    const char *generation = arg;

    printf("[task] Running inside %s.\n", generation);
}

static void
run_generation(const char *generation)
{
    printf("[example] Starting %s...\n", generation);

    gt_status_t status = gt_init();
    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Runtime initialization failed: %s\n", gt_status_string(status));
        return;
    }

    status = gt_create_task(worker_task_entry, (void *)generation, STANDARD_STACK_SIZE);

    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Failed to create task: %s\n", gt_status_string(status));
        gt_destroy();
        return;
    }

    gt_run();

    printf("[example] Shutting down %s...\n", generation);

    gt_destroy();
}

int
main(void)
{
    printf("[example] Demonstrating runtime reinitialization...\n");

    run_generation("Generation 1");
    run_generation("Generation 2");

    printf("[example] Runtime successfully completed two independent lifecycle cycles.\n");

    return 0;
}
