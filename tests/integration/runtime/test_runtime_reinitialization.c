#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/types.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/task/task.h>

struct reinit_test_payload
{
    volatile bool *executed;
    volatile gt_task_id_t *captured_id;
};

static volatile bool g_gen1_task_executed = false;
static volatile bool g_gen2_task_executed = false;

static volatile gt_task_id_t g_gen1_task_id = UINT32_MAX;
static volatile gt_task_id_t g_gen2_task_id = UINT32_MAX;

static void
reinit_test_worker_entry(void *arg)
{
    struct reinit_test_payload *payload = arg;

    assert(payload != NULL);

    *payload->executed = true;

    if (runtime.rt_current != NULL)
    {
        *payload->captured_id = runtime.rt_current->t_id;
    }
}

/*
 * Verifies that the runtime can be initialized, executed, destroyed,
 * and initialized again without leaking state across generations.
 *
 * Ensures that:
 *
 *   - runtime state is fully reset after destruction
 *   - task IDs are recycled from a clean allocator
 *   - scheduler queues begin empty after reinitialization
 *   - independent runtime generations execute correctly
 */
static void
test_runtime_reinitialization(void)
{
    const size_t standard_stack_size = 16 * 1024;
    gt_status_t status;

    g_gen1_task_executed = false;
    g_gen2_task_executed = false;

    g_gen1_task_id = UINT32_MAX;
    g_gen2_task_id = UINT32_MAX;

    /*
     * ----------------------------------------------------------------------
     * Generation 1
     * ----------------------------------------------------------------------
     */

    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_state == GT_RUNTIME_STATE_INITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    struct reinit_test_payload gen1_payload = {
        .executed = &g_gen1_task_executed,
        .captured_id = &g_gen1_task_id,
    };

    status = gt_runtime_create_task(reinit_test_worker_entry, &gen1_payload, standard_stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 1);

    gt_runtime_run();

    assert(g_gen1_task_executed == true);
    assert(g_gen1_task_id == 0);

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    /*
     * ----------------------------------------------------------------------
     * Shutdown
     * ----------------------------------------------------------------------
     */

    gt_runtime_destroy();

    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    /*
     * ----------------------------------------------------------------------
     * Generation 2
     * ----------------------------------------------------------------------
     */

    status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_state == GT_RUNTIME_STATE_INITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    struct reinit_test_payload gen2_payload = {
        .executed = &g_gen2_task_executed,
        .captured_id = &g_gen2_task_id,
    };

    status = gt_runtime_create_task(reinit_test_worker_entry, &gen2_payload, standard_stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 1);

    gt_runtime_run();

    assert(g_gen2_task_executed == true);
    assert(g_gen2_task_id == 0);

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    /*
     * ----------------------------------------------------------------------
     * Final Shutdown
     * ----------------------------------------------------------------------
     */

    gt_runtime_destroy();

    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);
}

int
main(void)
{
    puts("[RUN] integration/runtime_reinitialization");

    test_runtime_reinitialization();

    puts("[PASS] integration/runtime_reinitialization");

    return 0;
}
