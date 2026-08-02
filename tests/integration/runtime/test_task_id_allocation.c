#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/common/types.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/task/task.h>

static volatile gt_task_id_t g_generation1_task0_id = 999;
static volatile gt_task_id_t g_generation1_task1_id = 999;
static volatile gt_task_id_t g_generation1_task2_id = 999;

static volatile gt_task_id_t g_generation2_task0_id = 999;
static volatile gt_task_id_t g_generation2_task1_id = 999;

static void
capture_task_id_entry(void *arg)
{
    gt_task_id_t *captured_id = arg;

    assert(runtime.rt_current != NULL);

    *captured_id = runtime.rt_current->t_id;
}

/*
 * Verifies that task identifiers are allocated sequentially and recycled
 * after every completed runtime execution.
 */
static void
test_task_identifier_allocation(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const size_t standard_stack_sz = 16 * 1024;

    /*
     * ------------------------------------------------------------------
     * Generation #1
     * ------------------------------------------------------------------
     */

    status = gt_runtime_create_task(
        capture_task_id_entry, (void *)&g_generation1_task0_id, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(
        capture_task_id_entry, (void *)&g_generation1_task1_id, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(
        capture_task_id_entry, (void *)&g_generation1_task2_id, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 3);

    gt_runtime_run();

    assert(g_generation1_task0_id == 0);
    assert(g_generation1_task1_id == 1);
    assert(g_generation1_task2_id == 2);

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    /*
     * ------------------------------------------------------------------
     * Generation #2
     * ------------------------------------------------------------------
     */

    status = gt_runtime_create_task(
        capture_task_id_entry, (void *)&g_generation2_task0_id, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(
        capture_task_id_entry, (void *)&g_generation2_task1_id, standard_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 2);

    gt_runtime_run();

    /*
     * Released identifiers should be recycled from the lowest available
     * values.
     */
    assert(g_generation2_task0_id == 0);
    assert(g_generation2_task1_id == 1);

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
    puts("[RUN] integration/runtime_task_id_allocation");

    test_task_identifier_allocation();

    puts("[PASS] integration/runtime_task_id_allocation");
    return 0;
}
