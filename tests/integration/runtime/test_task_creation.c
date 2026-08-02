#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/macros.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/task/task.h>

/**
 * Shared payload used to verify task execution.
 */
static int g_test_payload = 0;

/**
 * Simple integration task.
 *
 * Increments the supplied integer exactly once before returning.
 */
static void
dummy_entry(void *arg)
{
    int *counter = (int *)arg;
    (*counter)++;
}

/**
 * @brief Verifies the complete lifecycle of a runtime task.
 *
 * This test exercises the entire runtime pipeline:
 *
 *   - runtime initialization
 *   - task creation
 *   - scheduler admission
 *   - task execution
 *   - task termination
 *   - zombie reaping
 *   - runtime cleanup
 *
 * The runtime should return to an idle state with no remaining tasks.
 */
static void
test_task_creation_and_execution_cleanup(void)
{
    gt_status_t status;

    g_test_payload = 0;

    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 0);
    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    status = gt_runtime_create_task(dummy_entry, &g_test_payload, 16 * 1024);

    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 1);
    assert(!gt_list_is_empty(&runtime.rt_tasks));

    const struct gt_list_node *node = gt_list_front(&runtime.rt_tasks);
    assert(node != NULL);

    const struct gt_task *task = GT_CONTAINER_OF_SAFE(node, struct gt_task, t_runtime_node);

    assert(task != NULL);
    assert(task->t_id == 0);
    assert(task->t_entry == dummy_entry);
    assert(task->t_arg == &g_test_payload);
    assert(task->t_state == GT_TASK_STATE_READY);

    /*
     * Execute every runnable task until the runtime naturally returns to
     * the caller.
     */
    gt_runtime_run();

    /*
     * The task must have executed exactly once and all runtime-owned
     * resources should have been reclaimed.
     */
    assert(g_test_payload == 1);

    assert(runtime.rt_task_count == 0);
    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();
}

int
main(void)
{
    puts("[RUN] integration/runtime_task_creation");

    test_task_creation_and_execution_cleanup();

    puts("[PASS] integration/runtime_task_creation");
    return 0;
}
