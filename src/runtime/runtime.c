#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/common/mem.h>
#include <gt_internal/ds/id_pool/id_pool.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/platform/platform.h>
#include <gt_internal/scheduler/scheduler.h>
#include <gt_internal/task/task.h>

/*
 * Singleton runtime instance.
 */
struct gt_runtime runtime;

/*
 * --------------------------------------------------------------------------
 * Validation helpers
 * --------------------------------------------------------------------------
 */

static GT_FORCE_INLINE void
__gt_runtime_validate_initialized(void)
{
    GT_ASSERT(runtime.rt_state == GT_RUNTIME_STATE_UNINITIALIZED);
}

static GT_FORCE_INLINE void
__gt_runtime_validate_running_state(void)
{
    GT_ASSERT(runtime.rt_state != GT_RUNTIME_STATE_UNINITIALIZED);
}

/*
 * --------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------
 */

gt_status_t
gt_runtime_init(void)
{
    gt_status_t status;

    __gt_runtime_validate_initialized();

    gt_mem_clear(&runtime, sizeof(runtime));

    status = gt_platform_init();
    if (status != GT_STATUS_SUCCESS)
    {
        return status;
    }

    status = gt_scheduler_init(&runtime.rt_scheduler);
    if (status != GT_STATUS_SUCCESS)
    {
        gt_platform_destroy();
        return status;
    }

    gt_list_init(&runtime.rt_tasks);
    gt_list_init(&runtime.rt_zombies);

    gt_id_pool_init(&runtime.rt_task_ids, runtime.rt_task_id_storage, GT_TASK_MAX_CAPACITY);

    gt_mem_clear(&runtime.rt_main_context, sizeof(runtime.rt_main_context));

    runtime.rt_task_count = 0;
    runtime.rt_current = NULL;
    runtime.rt_state = GT_RUNTIME_STATE_INITIALIZED;

    return GT_STATUS_SUCCESS;
}

gt_status_t
gt_runtime_create_task(gt_task_entry_fn entry, void *arg, size_t stack_size)
{
    __gt_runtime_validate_running_state();

    return __gt_runtime_task_create(entry, arg, stack_size);
}

void
gt_runtime_destroy(void)
{
    __gt_runtime_validate_running_state();

    __gt_runtime_destroy_task_list(&runtime.rt_tasks);

    __gt_runtime_reap_zombies();

    gt_scheduler_destroy(&runtime.rt_scheduler);

    gt_platform_destroy();

    gt_mem_clear(&runtime, sizeof(runtime));
}
