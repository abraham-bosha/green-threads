#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/ds/list/list.h>

/*
 * Verifies that executing an initialized runtime with no runnable tasks
 * immediately terminates without modifying runtime state.
 */
static void
test_empty_runtime_execution(void)
{
    gt_status_t GT_MAYBE_UNUSED status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_state == GT_RUNTIME_STATE_INITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    /*
     * Running an empty runtime should immediately return.
     */
    gt_runtime_run();

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();

    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(runtime.rt_tasks.head.next == NULL);
    assert(runtime.rt_tasks.head.prev == NULL);

    assert(runtime.rt_zombies.head.next == NULL);
    assert(runtime.rt_zombies.head.prev == NULL);
}

int
main(void)
{
    puts("[RUN] integration/runtime_empty");

    test_empty_runtime_execution();

    puts("[PASS] integration/runtime_empty");

    return 0;
}
