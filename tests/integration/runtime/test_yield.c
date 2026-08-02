#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>

static volatile int g_yielding_task_progress = 0;
static volatile int g_sibling_task_progress = 0;
static volatile bool g_task_resumed_after_yield = false;

static void
yielding_task_entry(void *arg)
{
    (void)arg;

    /*
     * Record that the first task has started, then voluntarily yield
     * execution so another runnable task may execute.
     */
    g_yielding_task_progress = 1;

    gt_runtime_yield();

    /*
     * Execution should resume only after the sibling task has completed.
     */
    assert(g_sibling_task_progress == 2);

    g_task_resumed_after_yield = true;

    /*
     * Mark successful completion of the yielding task.
     */
    g_yielding_task_progress = 3;
}

static void
sibling_task_entry(void *arg)
{
    (void)arg;

    /*
     * The sibling task must execute only after the first task yields.
     */
    assert(g_yielding_task_progress == 1);
    assert(g_task_resumed_after_yield == false);

    g_sibling_task_progress = 2;
}

static void
test_cooperative_task_yield(void)
{
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const size_t stack_size = 16384UL;

    status = gt_runtime_create_task(yielding_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(sibling_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 2);

    /*
     * Execute every runnable task until the scheduler becomes idle.
     */
    gt_runtime_run();

    /*
     * Verify execution ordering.
     */
    assert(g_yielding_task_progress == 3);
    assert(g_sibling_task_progress == 2);
    assert(g_task_resumed_after_yield == true);

    /*
     * Verify runtime cleanup after execution completes.
     */
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
    puts("[RUN] integration/runtime_yield");

    test_cooperative_task_yield();

    puts("[PASS] integration/runtime_yield");
    return 0;
}
