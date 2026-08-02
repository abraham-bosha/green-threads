#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

/**
 * @brief Verifies the runtime initialization and destruction lifecycle.
 *
 * This test validates the runtime state machine across the complete
 * initialization sequence:
 *
 *   - initial zero state
 *   - successful runtime initialization
 *   - successful runtime destruction
 *
 * The runtime must begin and end in an uninitialized state with no
 * active tasks or scheduler ownership.
 */
static void
test_runtime_lifecycle_invariants(void)
{
    gt_status_t status;

    /*
     * Verify the runtime begins in its pristine zero-initialized state.
     */
    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    /*
     * Initialize the runtime.
     */
    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    /*
     * Verify initialization established the expected runtime state.
     */
    assert(runtime.rt_state == GT_RUNTIME_STATE_INITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    /*
     * Destroy the runtime and verify every subsystem has been reset.
     */
    gt_runtime_destroy();

    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);
}

int
main(void)
{
    puts("[RUN] integration/runtime_init");

    test_runtime_lifecycle_invariants();

    puts("[PASS] integration/runtime_init");
    return 0;
}
