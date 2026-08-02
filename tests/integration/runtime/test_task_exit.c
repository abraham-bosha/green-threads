#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/scheduler/scheduler.h>
#include <gt_internal/task/task.h>

/*
 * Execution checkpoints used to verify that the expected control flow
 * occurred during the integration test.
 */
static volatile int g_exiting_task_completed = 0;
static volatile bool g_zombie_observed = false;

/*
 * Executes after the first task has already exited.
 *
 * At this point the runtime should have:
 *
 *   - moved the completed task into the zombie list,
 *   - preserved its metadata,
 *   - postponed destruction until execution returns to the runtime.
 */
static void
inspector_task_entry(void *arg)
{
    (void)arg;

    assert(gt_list_is_empty(&runtime.rt_zombies) == false);

    const struct gt_list_node *node = gt_list_front(&runtime.rt_zombies);
    assert(node != NULL);

    const struct gt_task *zombie = GT_CONTAINER_OF_SAFE(node, struct gt_task, t_runtime_node);

    assert(zombie != NULL);
    assert(zombie->t_id == 0);
    assert(zombie->t_state == GT_TASK_STATE_ZOMBIE);

    g_zombie_observed = true;
}

/*
 * Simple task whose completion should automatically transition it into
 * the zombie state.
 */
static void
exiting_task_entry(void *arg)
{
    (void)arg;

    g_exiting_task_completed = 1;
}

/*
 * Verifies the complete automatic task-exit lifecycle:
 *
 *   READY
 *       ↓
 *   RUNNING
 *       ↓
 *   ZOMBIE
 *       ↓
 *   REAPED
 */
static void
test_automatic_task_exit_mechanics(void)
{
    gt_status_t GT_MAYBE_UNUSED status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const size_t stack_size = 16384UL;

    status = gt_runtime_create_task(exiting_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(inspector_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 2);

    /*
     * Run until every runnable task completes.
     *
     * The first task should become a zombie before the second task
     * executes, allowing the inspector to validate the intermediate
     * runtime state.
     */
    gt_runtime_run();

    assert(g_exiting_task_completed == 1);
    assert(g_zombie_observed == true);

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);

    /*
     * All zombies should have been reaped before control returns to
     * the caller.
     */
    assert(runtime.rt_task_count == 0);
    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();
}

int
main(void)
{
    puts("[RUN] integration/runtime_task_exit");

    test_automatic_task_exit_mechanics();

    puts("[PASS] integration/runtime_task_exit");

    return 0;
}
