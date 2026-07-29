#include <gt_internal/task/task.h>

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/mem.h>
#include <gt_internal/common/types.h>
#include <gt_internal/memory/stack/stack.h>
#include <gt_internal/platform/context/context.h>

static GT_FORCE_INLINE void
__gt_task_validate_task(const struct gt_task *GT_MAYBE_UNUSED task)
{
    GT_ASSERT(task != NULL);
}

static GT_FORCE_INLINE void
__gt_task_validate_entry(gt_task_entry_fn GT_MAYBE_UNUSED entry)
{
    GT_ASSERT(entry != NULL);
}

static GT_FORCE_INLINE void
__gt_task_clear(struct gt_task *task)
{
    gt_mem_clear(task, sizeof(*task));
}

gt_status_t
gt_task_init(
    struct gt_task *task, gt_task_id_t id, gt_task_entry_fn entry, void *arg, size_t stack_size)
{
    __gt_task_validate_task(task);
    __gt_task_validate_entry(entry);
    __gt_task_clear(task);

    gt_status_t status;

    task->t_id = id;
    task->t_state = GT_TASK_STATE_NEW;

    task->t_entry = entry;
    task->t_arg = arg;

    status = gt_stack_init(&task->t_stack, stack_size);
    if (status != GT_STATUS_SUCCESS)
    {
        return status;
    }

    status = gt_context_init(&task->t_context, task->t_stack.s_base, task->t_stack.s_size);
    if (status != GT_STATUS_SUCCESS)
    {
        gt_stack_destroy(&task->t_stack);
        __gt_task_clear(task);
        return status;
    }

    return GT_STATUS_SUCCESS;
}

void
gt_task_destroy(struct gt_task *task)
{
    __gt_task_validate_task(task);

    gt_context_destroy(&task->t_context);
    gt_stack_destroy(&task->t_stack);

    __gt_task_clear(task);
}
