#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/ds/list/list.h>

static volatile bool g_stack_4kb_verified = false;
static volatile bool g_stack_8kb_verified = false;
static volatile bool g_stack_16kb_verified = false;
static volatile bool g_stack_64kb_verified = false;

static void
task_stack_4kb_entry(void *arg)
{
    const int *value = arg;

    assert(*value == 4);

    g_stack_4kb_verified = true;
}

static void
task_stack_8kb_entry(void *arg)
{
    const int *value = arg;

    assert(*value == 8);

    g_stack_8kb_verified = true;
}

static void
task_stack_16kb_entry(void *arg)
{
    const int *value = arg;

    assert(*value == 16);

    g_stack_16kb_verified = true;
}

static void
task_stack_64kb_entry(void *arg)
{
    const int *value = arg;

    assert(*value == 64);

    g_stack_64kb_verified = true;
}

/*
 * Verifies that the runtime correctly allocates, executes, and reclaims
 * tasks created with different requested stack sizes.
 */
static void
test_task_stack_sizes(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const int value_4kb = 4;
    const int value_8kb = 8;
    const int value_16kb = 16;
    const int value_64kb = 64;

    status = gt_runtime_create_task(task_stack_4kb_entry, (void *)&value_4kb, 4UL * 1024UL);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_stack_8kb_entry, (void *)&value_8kb, 8UL * 1024UL);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_stack_16kb_entry, (void *)&value_16kb, 16UL * 1024UL);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_stack_64kb_entry, (void *)&value_64kb, 64UL * 1024UL);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 4);

    /* Execute every task. */
    gt_runtime_run();

    /* Every task must have completed successfully. */
    assert(g_stack_4kb_verified);
    assert(g_stack_8kb_verified);
    assert(g_stack_16kb_verified);
    assert(g_stack_64kb_verified);

    /* The runtime should be completely drained. */
    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();
}

int
main(void)
{
    puts("[RUN] integration/runtime_task_stack_sizes");

    test_task_stack_sizes();

    puts("[PASS] integration/runtime_task_stack_sizes");
    return 0;
}
