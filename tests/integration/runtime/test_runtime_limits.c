#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/ds/list/list.h>

static volatile int g_execution_count = 0;

static void
capacity_test_task_entry(void *arg)
{
    (void)arg;

    g_execution_count++;
}

/*
 * Verifies that the runtime enforces its configured task capacity.
 *
 * Tasks should be admitted successfully until the runtime reaches
 * GT_TASK_MAX_CAPACITY. Any further creation attempt must fail without
 * altering runtime state. Every successfully admitted task must still
 * execute normally.
 */
static void
test_runtime_task_capacity(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const size_t stack_size = 16 * 1024;

    for (size_t i = 0; i < GT_TASK_MAX_CAPACITY; ++i)
    {
        status = gt_runtime_create_task(capacity_test_task_entry, NULL, stack_size);

        assert(status == GT_STATUS_SUCCESS);
    }

    assert(runtime.rt_task_count == GT_TASK_MAX_CAPACITY);

    /*
     * The runtime should reject any task beyond its configured limit.
     */
    status = gt_runtime_create_task(capacity_test_task_entry, NULL, stack_size);

    assert(status == GT_STATUS_RESOURCE_EXHAUSTED);

    assert(runtime.rt_task_count == GT_TASK_MAX_CAPACITY);

    gt_runtime_run();

    assert(g_execution_count == GT_TASK_MAX_CAPACITY);

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
    puts("[RUN] integration/runtime_limits");

    test_runtime_task_capacity();

    puts("[PASS] integration/runtime_limits");

    return 0;
}
