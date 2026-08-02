#include <stdbool.h>
#include <stdio.h>

#include <gt/gt.h>

#define PIPELINE_ITEMS 3

struct pipeline_channel
{
    int raw_value;
    int processed_value;

    bool producer_done;
    bool processor_done;
};

static void
producer_task_entry(void *arg)
{
    struct pipeline_channel *channel = arg;

    for (int value = 1; value <= PIPELINE_ITEMS; ++value)
    {
        channel->raw_value = value;

        printf("[producer ] Produced %d\n", value);

        gt_yield();
    }

    channel->producer_done = true;

    printf("[producer ] Finished.\n");
}

static void
processor_task_entry(void *arg)
{
    struct pipeline_channel *channel = arg;

    while (!channel->producer_done || channel->raw_value != 0)
    {
        if (channel->raw_value != 0)
        {
            channel->processed_value = channel->raw_value * channel->raw_value;

            printf("[processor] %d -> %d\n", channel->raw_value, channel->processed_value);

            channel->raw_value = 0;
        }

        gt_yield();
    }

    channel->processor_done = true;

    printf("[processor] Finished.\n");
}

static void
consumer_task_entry(void *arg)
{
    struct pipeline_channel *channel = arg;

    while (!channel->processor_done || channel->processed_value != 0)
    {
        if (channel->processed_value != 0)
        {
            printf("[consumer ] Consumed %d\n", channel->processed_value);

            channel->processed_value = 0;
        }

        gt_yield();
    }

    printf("[consumer ] Finished.\n");
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

    struct pipeline_channel channel = {
        .raw_value = 0,
        .processed_value = 0,
        .producer_done = false,
        .processor_done = false,
    };

    printf("[example] Creating pipeline tasks...\n");

    status = gt_create_task(producer_task_entry, &channel, stack_size);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(processor_task_entry, &channel, stack_size);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    status = gt_create_task(consumer_task_entry, &channel, stack_size);
    if (status != GT_STATUS_SUCCESS)
        goto cleanup;

    printf("[example] Starting scheduler...\n\n");

    gt_run();

    printf("\n[example] Pipeline completed successfully.\n");

    gt_destroy();

    printf("[example] Runtime shut down successfully.\n");

    return 0;

cleanup:
    fprintf(stderr, "[error] Failed to create task: %s\n", gt_status_string(status));

    gt_destroy();
    return 1;
}
