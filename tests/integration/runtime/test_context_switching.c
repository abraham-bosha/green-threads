#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/list/list.h>

static volatile int g_task_zero_switch_count = 0;
static volatile int g_task_one_switch_count = 0;

static void
context_worker_zero_entry(void *GT_MAYBE_UNUSED arg)
{
    volatile uint64_t sentinel_a = 0xAAAAAAAAAAAAAAAAULL;
    volatile uint64_t sentinel_b = 0xBBBBBBBBBBBBBBBBULL;

    g_task_zero_switch_count = 1;

    gt_runtime_yield();

    assert(sentinel_a == 0xAAAAAAAAAAAAAAAAULL);
    assert(sentinel_b == 0xBBBBBBBBBBBBBBBBULL);

    g_task_zero_switch_count = 2;

    gt_runtime_yield();

    assert(sentinel_a == 0xAAAAAAAAAAAAAAAAULL);
    assert(sentinel_b == 0xBBBBBBBBBBBBBBBBULL);
}

static void
context_worker_one_entry(void *GT_MAYBE_UNUSED arg)
{
    volatile uint64_t sentinel_x = 0x1111111111111111ULL;
    volatile uint64_t sentinel_y = 0x2222222222222222ULL;

    g_task_one_switch_count = 1;

    gt_runtime_yield();

    assert(sentinel_x == 0x1111111111111111ULL);
    assert(sentinel_y == 0x2222222222222222ULL);

    g_task_one_switch_count = 2;

    gt_runtime_yield();

    assert(sentinel_x == 0x1111111111111111ULL);
    assert(sentinel_y == 0x2222222222222222ULL);
}

/*
 * Verifies that execution contexts preserve each task's stack frame across
 * multiple cooperative context switches.
 *
 * Ensures that:
 *
 *   - local stack variables remain intact after yielding
 *   - multiple tasks resume with their original stack contents
 *   - repeated context switches do not corrupt execution state
 */
static void
test_context_switching(void)
{
    const size_t standard_stack_size = 16 * 1024;

    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    g_task_zero_switch_count = 0;
    g_task_one_switch_count = 0;

    status = gt_runtime_create_task(context_worker_zero_entry, NULL, standard_stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(context_worker_one_entry, NULL, standard_stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 2);

    gt_runtime_run();

    assert(g_task_zero_switch_count == 2);
    assert(g_task_one_switch_count == 2);

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
    puts("[RUN] integration/runtime_context_switching");

    test_context_switching();

    puts("[PASS] integration/runtime_context_switching");

    return 0;
}
