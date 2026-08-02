#include <gt_internal/runtime/runtime_internal.h>

#include <stdbool.h>
#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/platform/heap/heap.h>
#include <gt_internal/task/task.h>

/*
 * --------------------------------------------------------------------------
 * Validation helpers
 * --------------------------------------------------------------------------
 */

static GT_FORCE_INLINE void
__gt_runtime_validate_task(const struct gt_task *GT_MAYBE_UNUSED task)
{
    GT_ASSERT(task != NULL);
}

/*
 * --------------------------------------------------------------------------
 * Internal helpers
 * --------------------------------------------------------------------------
 */

static gt_status_t
__gt_runtime_allocate_task(struct gt_task **task)
{
    return gt_heap_alloc((void **)task, sizeof(struct gt_task));
}

static void
__gt_runtime_free_task(struct gt_task *task)
{
    gt_heap_free(task);
}

static gt_status_t
__gt_runtime_allocate_task_id(gt_task_id_t *id)
{
    bool allocated;

    allocated = gt_id_pool_allocate(&runtime.rt_task_ids, id);

    if (!allocated)
    {
        return GT_STATUS_RESOURCE_EXHAUSTED;
    }

    return GT_STATUS_SUCCESS;
}

static void
__gt_runtime_release_task_id(gt_task_id_t id)
{
    gt_id_pool_release(&runtime.rt_task_ids, id);
}

/*
 * --------------------------------------------------------------------------
 * Task lifetime
 * --------------------------------------------------------------------------
 */

gt_status_t
__gt_runtime_task_create(gt_task_entry_fn entry, void *arg, size_t stack_size)
{
    struct gt_task *task;
    gt_task_id_t id;
    gt_status_t status;

    task = NULL;

    status = __gt_runtime_allocate_task(&task);
    if (status != GT_STATUS_SUCCESS)
    {
        return status;
    }

    status = __gt_runtime_allocate_task_id(&id);
    if (status != GT_STATUS_SUCCESS)
    {
        __gt_runtime_free_task(task);
        return status;
    }

    status = gt_task_init(task, id, entry, arg, stack_size);

    if (status != GT_STATUS_SUCCESS)
    {
        __gt_runtime_release_task_id(id);
        __gt_runtime_free_task(task);
        return status;
    }

    status = gt_context_configure(&task->t_context, __gt_runtime_task_trampoline, task);
    if (status != GT_STATUS_SUCCESS)
    {
        gt_task_destroy(task);
        __gt_runtime_release_task_id(id);
        __gt_runtime_free_task(task);
        return status;
    }

    task->t_state = GT_TASK_STATE_READY;
    gt_list_push_back(&runtime.rt_tasks, &task->t_runtime_node);

    status = gt_scheduler_admit(&runtime.rt_scheduler, task);
    if (status != GT_STATUS_SUCCESS)
    {
        gt_list_remove(&task->t_runtime_node);

        gt_task_destroy(task);
        __gt_runtime_release_task_id(id);
        __gt_runtime_free_task(task);

        return status;
    }

    runtime.rt_task_count++;

    return GT_STATUS_SUCCESS;
}

void
__gt_runtime_destroy_task_list(struct gt_list *list)
{
    struct gt_list_node *node;

    while ((node = gt_list_front(list)) != NULL)
    {
        struct gt_task *task;

        task = GT_CONTAINER_OF(node, struct gt_task, t_runtime_node);

        __gt_runtime_task_destroy(task);
    }
}

void
__gt_runtime_reap_zombies(void)
{
    __gt_runtime_destroy_task_list(&runtime.rt_zombies);
}

void
__gt_runtime_task_destroy(struct gt_task *task)
{
    __gt_runtime_validate_task(task);

    gt_list_remove(&task->t_runtime_node);

    gt_task_id_t id = task->t_id;

    gt_task_destroy(task);

    __gt_runtime_release_task_id(id);

    __gt_runtime_free_task(task);

    GT_ASSERT(runtime.rt_task_count > 0);

    runtime.rt_task_count--;
}
