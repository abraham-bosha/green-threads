#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/task/task.h>

static struct gt_task *g_target_task = NULL;
static volatile int g_lifecycle_checkpoint = 0;

/*
 * Observes the target task from a separate execution context and verifies
 * externally visible lifecycle transitions.
 */
static void
lifecycle_inspector_entry(void *arg)
{
    (void)arg;

    assert(g_target_task != NULL);

    assert(g_lifecycle_checkpoint == 1);
    assert(g_target_task->t_state == GT_TASK_STATE_READY);

    g_lifecycle_checkpoint = 2;

    gt_runtime_yield();

    assert(g_lifecycle_checkpoint == 3);
    assert(g_target_task->t_state == GT_TASK_STATE_ZOMBIE);
    assert(!gt_list_is_empty(&runtime.rt_zombies));

    g_lifecycle_checkpoint = 4;
}

/*
 * Exercises the runnable lifecycle of the target task.
 */
static void
lifecycle_target_entry(void *arg)
{
    (void)arg;

    assert(g_target_task != NULL);

    assert(runtime.rt_current == g_target_task);
    assert(g_target_task->t_state == GT_TASK_STATE_RUNNING);

    g_lifecycle_checkpoint = 1;

    gt_runtime_yield();

    assert(runtime.rt_current == g_target_task);
    assert(g_target_task->t_state == GT_TASK_STATE_RUNNING);

    g_lifecycle_checkpoint = 3;
}

/*
 * Verifies the observable lifecycle of a task:
 *
 *      READY → RUNNING → READY → RUNNING → ZOMBIE
 *
 * and confirms that zombie tasks remain visible until the runtime finishes
 * execution and performs reclamation.
 */
static void
test_task_state_transitions(void)
{
    gt_status_t GT_MAYBE_UNUSED status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    g_lifecycle_checkpoint = 0;

    const size_t standard_stack_sz = 16 * 1024;

    /*
     * Create the target task and retain a stable reference for later
     * inspection.
     */
    status = gt_runtime_create_task(lifecycle_target_entry, NULL, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    struct gt_list_node *node = gt_list_front(&runtime.rt_tasks);
    assert(node != NULL);

    g_target_task = GT_CONTAINER_OF_SAFE(node, struct gt_task, t_runtime_node);
    assert(g_target_task != NULL);

    assert(g_target_task->t_state == GT_TASK_STATE_READY);

    /*
     * Create the observer task.
     */
    status = gt_runtime_create_task(lifecycle_inspector_entry, NULL, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 2);

    /*
     * Execute both tasks.
     */
    gt_runtime_run();

    /*
     * Verify the complete lifecycle completed successfully.
     */
    assert(g_lifecycle_checkpoint == 4);

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
    puts("[RUN] integration/runtime_task_lifecycle");

    test_task_state_transitions();

    puts("[PASS] integration/runtime_task_lifecycle");

    return 0;
}
