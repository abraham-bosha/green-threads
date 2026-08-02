#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/gt.h>

struct custom_payload
{
    uint32_t magic;
    uint64_t data;
    char flag;
};

static void
task_integer_entry(void *arg)
{
    const int *value = arg;

    printf("[task integer] Value: %d\n", *value);
}

static void
task_structure_entry(void *arg)
{
    const struct custom_payload *payload = arg;

    printf("[task structure] Magic: 0x%X\n", payload->magic);
    printf("[task structure] Data : 0x%" PRIX64 "\n", payload->data);
    printf("[task structure] Flag : '%c'\n", payload->flag);
}

static void
task_null_entry(void *arg)
{
    (void)arg;

    printf("[task null] Argument: NULL\n");
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

    /* Example payloads stored on the main thread's stack. */
    int integer_data = 8888;

    struct custom_payload structure_data = {
        .magic = 0xBADCAFE,
        .data = UINT64_C(0xBADBEEF),
        .flag = 'G',
    };

    printf("[example] Scheduling tasks with different argument types...\n");

    status = gt_create_task(task_integer_entry, &integer_data, stack_size);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(task_structure_entry, &structure_data, stack_size);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(task_null_entry, NULL, stack_size);
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
