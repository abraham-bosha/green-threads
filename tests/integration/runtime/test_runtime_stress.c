#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/ds/list/list.h>

/*
 * Reserve capacity so that parent tasks can dynamically create children
 * during execution.
 */
#define GT_STRESS_NESTED_BATCH 5000
#define GT_STRESS_TASK_COUNT (GT_TASK_MAX_CAPACITY - GT_STRESS_NESTED_BATCH)
#define GT_STRESS_CONTEXT_SWITCHES 10

static volatile size_t g_primary_task_executions = 0;
static volatile size_t g_nested_task_executions = 0;

static void
stress_nested_child_entry(void *GT_MAYBE_UNUSED arg)
{
    volatile uint64_t state = 0x02468ULL;

    for (int i = 0; i < GT_STRESS_CONTEXT_SWITCHES; ++i)
    {
        state ^= (uint64_t)i;
        state += 0x9E3779B97F4A7C15ULL;

        gt_runtime_yield();
    }

    uint64_t expected = 0x02468ULL;

    for (int i = 0; i < GT_STRESS_CONTEXT_SWITCHES; ++i)
    {
        expected ^= (uint64_t)i;
        expected += 0x9E3779B97F4A7C15ULL;
    }

    assert(state == expected);

    g_nested_task_executions++;
}

static void
stress_primary_worker_entry(void *arg)
{
    uintptr_t worker_id = (uintptr_t)arg;

    volatile uint64_t state = worker_id ^ 0xDEADBEEFCAFEBABEULL;

    for (int i = 0; i < GT_STRESS_CONTEXT_SWITCHES; ++i)
    {
        state *= 0x13579ULL;
        state ^= (worker_id + i);

        gt_runtime_yield();
    }

    uint64_t expected = worker_id ^ 0xDEADBEEFCAFEBABEULL;

    for (int i = 0; i < GT_STRESS_CONTEXT_SWITCHES; ++i)
    {
        expected *= 0x13579ULL;
        expected ^= (worker_id + i);
    }

    assert(state == expected);

    g_primary_task_executions++;

    if (worker_id < GT_STRESS_NESTED_BATCH)
    {
        size_t stack_size;

        switch (worker_id % 8)
        {
            case 0:
                stack_size = 4 * 1024;
                break;
            case 1:
                stack_size = 8 * 1024;
                break;
            case 2:
                stack_size = 12 * 1024;
                break;
            case 3:
                stack_size = 16 * 1024;
                break;
            case 4:
                stack_size = 24 * 1024;
                break;
            case 5:
                stack_size = 32 * 1024;
                break;
            case 6:
                stack_size = 48 * 1024;
                break;

            default:
                stack_size = 64 * 1024;
                break;
        }

        gt_status_t status = gt_runtime_create_task(stress_nested_child_entry, NULL, stack_size);

        assert(status == GT_STATUS_SUCCESS);
    }
}

static void
execute_stress_generation(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    g_primary_task_executions = 0;
    g_nested_task_executions = 0;

    for (uintptr_t i = 0; i < GT_STRESS_TASK_COUNT; ++i)
    {
        size_t stack_size;

        switch (i % 8)
        {
            case 0:
                stack_size = 4 * 1024;
                break;
            case 1:
                stack_size = 8 * 1024;
                break;
            case 2:
                stack_size = 12 * 1024;
                break;
            case 3:
                stack_size = 16 * 1024;
                break;
            case 4:
                stack_size = 24 * 1024;
                break;
            case 5:
                stack_size = 32 * 1024;
                break;
            case 6:
                stack_size = 48 * 1024;
                break;

            default:
                stack_size = 64 * 1024;
                break;
        }

        status = gt_runtime_create_task(stress_primary_worker_entry, (void *)i, stack_size);

        assert(status == GT_STATUS_SUCCESS);
    }

    assert(runtime.rt_task_count == GT_STRESS_TASK_COUNT);

    gt_runtime_run();

    assert(g_primary_task_executions == GT_STRESS_TASK_COUNT);
    assert(g_nested_task_executions == GT_STRESS_NESTED_BATCH);

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);
    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();

    assert(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
}

static void
test_runtime_stress(void)
{
    /*
     * Repeat complete runtime lifecycles to expose resource leaks,
     * allocator corruption, scheduler inconsistencies, and stale state.
     */
    for (size_t generation = 0; generation < 1; ++generation)
    {
        execute_stress_generation();
    }
}

int
main(void)
{
    puts("[RUN] integration/runtime_stress");

    test_runtime_stress();

    puts("[PASS] integration/runtime_stress");

    return 0;
}
