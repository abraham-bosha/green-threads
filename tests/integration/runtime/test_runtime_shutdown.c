#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/ds/list/list.h>

static volatile int g_execution_count = 0;

/*
 * Pending task that should never execute during this test.
 */
static void
pending_task_entry(void *arg)
{
    (void)arg;

    g_execution_count++;
}

/*
 * Verifies that destroying an initialized runtime correctly reclaims all
 * pending tasks without executing them and restores the runtime to its
 * uninitialized state.
 */
static void
test_runtime_destroy_with_pending_tasks(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const size_t stack_size = 16 * 1024;

    status = gt_runtime_create_task(pending_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(pending_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(pending_task_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 3);
    assert(gt_list_is_empty(&runtime.rt_tasks) == false);

    /*
     * Destroy the runtime before any task is scheduled.
     */
    gt_runtime_destroy();

    /*
     * No pending task should have executed.
     */
    assert(g_execution_count == 0);

    /*
     * Runtime should be completely reset.
     */
    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    /*
     * Internal containers should have been cleared.
     */
    assert(runtime.rt_tasks.head.next == NULL);
    assert(runtime.rt_tasks.head.prev == NULL);

    assert(runtime.rt_zombies.head.next == NULL);
    assert(runtime.rt_zombies.head.prev == NULL);

    assert(runtime.rt_scheduler.ready_queue.head.head.next == NULL);
    assert(runtime.rt_scheduler.ready_queue.head.head.prev == NULL);
}

int
main(void)
{
    puts("[RUN] integration/runtime_shutdown");

    test_runtime_destroy_with_pending_tasks();

    puts("[PASS] integration/runtime_shutdown");

    return 0;
}
