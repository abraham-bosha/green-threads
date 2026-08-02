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

static volatile int g_child_task_execution_count = 0;
static volatile bool g_child_task_created = false;

static const size_t g_stack_size = 16 * 1024;

/*
 * Child task created dynamically by the parent task.
 */
static void
child_task_entry(void *arg)
{
    int *payload = (int *)arg;

    g_child_task_execution_count = 1;

    *payload = 999;
}

/*
 * Parent task responsible for creating a child task while executing.
 */
static void
parent_task_entry(void *arg)
{
    int *child_payload = (int *)arg;

    const struct gt_task *current = runtime.rt_current;

    assert(current != NULL);
    assert(current->t_id == 0);

    assert(runtime.rt_task_count == 1);

    gt_status_t GT_MAYBE_UNUSED status =
        gt_runtime_create_task(child_task_entry, child_payload, g_stack_size);

    assert(status == GT_STATUS_SUCCESS);

    /*
     * The child should be admitted immediately as a runnable task.
     */
    assert(runtime.rt_task_count == 2);

    const struct gt_list_node *tail = gt_list_back(&runtime.rt_tasks);
    assert(tail != NULL);

    const struct gt_task *child = GT_CONTAINER_OF_SAFE(tail, struct gt_task, t_runtime_node);

    assert(child != NULL);
    assert(child->t_id == 1);
    assert(child->t_state == GT_TASK_STATE_READY);

    g_child_task_created = true;
}

/*
 * Verifies that a running task may safely create another runnable task.
 *
 * The child task should be admitted into the runtime immediately,
 * receive a fresh task identifier, and execute successfully after the
 * parent task finishes.
 */
static void
test_nested_task_creation(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    int payload = 0;

    status = gt_runtime_create_task(parent_task_entry, &payload, g_stack_size);

    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 1);

    gt_runtime_run();

    assert(g_child_task_created == true);
    assert(g_child_task_execution_count == 1);

    assert(payload == 999);

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
    puts("[RUN] integration/runtime_nested_task_creation");

    test_nested_task_creation();

    puts("[PASS] integration/runtime_nested_task_creation");

    return 0;
}
