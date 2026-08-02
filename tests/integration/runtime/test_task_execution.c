#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/ds/list/list.h>
#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>
#include <gt_internal/task/task.h>

/*
 * Shared state used to verify execution ordering and task completion.
 */
static volatile int g_task1_execution_counter = 0;
static volatile int g_task2_execution_counter = 0;

static volatile gt_task_id_t g_first_executed_task_id = 999;
static volatile bool g_initial_execution_captured = false;

/**
 * First task executed by the runtime.
 *
 * Records which task executes first and mutates the supplied payload to
 * prove that user task code was actually executed.
 */
static void
task_first_fifo_entry(void *arg)
{
    int *payload = (int *)arg;

    g_task1_execution_counter++;

    if (!g_initial_execution_captured)
    {
        if (runtime.rt_current != NULL)
        {
            g_first_executed_task_id = runtime.rt_current->t_id;
        }

        g_initial_execution_captured = true;
    }

    *payload = 888;
}

/**
 * Second task executed by the runtime.
 */
static void
task_second_fifo_entry(void *arg)
{
    (void)arg;

    g_task2_execution_counter++;
}

/**
 * @brief Verifies complete runtime task execution.
 *
 * This test exercises the complete execution engine by verifying:
 *
 *   - multiple task creation
 *   - scheduler admission
 *   - FIFO execution order
 *   - user task execution
 *   - runtime state transitions
 *   - automatic task destruction
 *   - zombie reaping
 */
static void
test_runtime_execution_engine(void)
{
    gt_status_t GT_MAYBE_UNUSED status;

    int task1_payload = 111;

    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    /*
     * Create two runnable tasks.
     */
    status = gt_runtime_create_task(task_first_fifo_entry, &task1_payload, 16 * 1024);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_second_fifo_entry, NULL, 16 * 1024);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 2);

    /*
     * Execute every runnable task until the runtime naturally returns to
     * the caller.
     */
    gt_runtime_run();

    /*
     * Verify runtime state after execution completes.
     */
    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);

    /*
     * Every task should execute exactly once.
     */
    assert(g_task1_execution_counter == 1);
    assert(g_task2_execution_counter == 1);

    /*
     * FIFO scheduling should execute the first admitted task first.
     */
    assert(g_initial_execution_captured);
    assert(g_first_executed_task_id == 0);

    /*
     * Verify user code executed successfully.
     */
    assert(task1_payload == 888);

    /*
     * Every runtime-owned task should have been reclaimed.
     */
    assert(runtime.rt_task_count == 0);
    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();
}

int
main(void)
{
    puts("[RUN] integration/runtime_task_execution");

    test_runtime_execution_engine();

    puts("[PASS] integration/runtime_task_execution");

    return 0;
}
