#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>

static volatile int g_execution_step = 0;

static void
task_zero_entry(void *arg)
{
    (void)arg;

    /* Round 1 */
    assert(g_execution_step == 0);
    g_execution_step = 1;

    gt_runtime_yield();

    /* Round 2 */
    assert(g_execution_step == 3);
    g_execution_step = 4;

    gt_runtime_yield();

    /* Final round */
    assert(g_execution_step == 6);
    g_execution_step = 7;
}

static void
task_one_entry(void *arg)
{
    (void)arg;

    /* Round 1 */
    assert(g_execution_step == 1);
    g_execution_step = 2;

    gt_runtime_yield();

    /* Round 2 */
    assert(g_execution_step == 4);
    g_execution_step = 5;

    gt_runtime_yield();

    /* Final round */
    assert(g_execution_step == 7);
    g_execution_step = 8;
}

static void
task_two_entry(void *arg)
{
    (void)arg;

    /* Round 1 */
    assert(g_execution_step == 2);
    g_execution_step = 3;

    gt_runtime_yield();

    /* Round 2 */
    assert(g_execution_step == 5);
    g_execution_step = 6;

    gt_runtime_yield();

    /* Final round */
    assert(g_execution_step == 8);
    g_execution_step = 9;
}

/*
 * Verifies cooperative round-robin scheduling.
 *
 * Three tasks repeatedly yield execution. The observed execution order
 * must remain:
 *
 *      Task0 → Task1 → Task2
 *      Task0 → Task1 → Task2
 *      Task0 → Task1 → Task2
 *
 * demonstrating that yielded tasks are re-admitted to the tail of the
 * ready queue while preserving FIFO scheduling order.
 */
static void
test_scheduler_round_robin(void)
{
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    g_execution_step = 0;

    const size_t stack_size = 16 * 1024;

    status = gt_runtime_create_task(task_zero_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_one_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_two_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 3);

    gt_runtime_run();

    assert(g_execution_step == 9);

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
    puts("[RUN] integration/runtime_scheduler_order");

    test_scheduler_round_robin();

    puts("[PASS] integration/runtime_scheduler_order");
    return 0;
}
