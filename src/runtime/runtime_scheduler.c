#include <gt_internal/runtime/runtime_internal.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/types.h>
#include <gt_internal/platform/context/context.h>
#include <gt_internal/scheduler/scheduler.h>
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
 * Scheduling helpers
 * --------------------------------------------------------------------------
 */

/*
 * Returns the next runnable task.
 */
static GT_FORCE_INLINE struct gt_task *
__gt_runtime_select_next(void)
{
    return gt_scheduler_next(&runtime.rt_scheduler);
}

/*
 * Resumes execution in the main thread after every runnable task has
 * completed.
 */
static GT_NORETURN void
__gt_runtime_resume_main(void)
{
    runtime.rt_current = NULL;
    runtime.rt_state = GT_RUNTIME_STATE_STOPPED;

    gt_context_load(&runtime.rt_main_context);

    GT_UNREACHABLE();
}

/*
 * Resumes execution of the supplied task.
 */
static GT_NORETURN void
__gt_runtime_resume_task(struct gt_task *task)
{
    __gt_runtime_validate_task(task);

    /* cppcheck-suppress nullPointerRedundantCheck */
    task->t_state = GT_TASK_STATE_RUNNING;

    runtime.rt_current = task;

    gt_context_load(&task->t_context);

    GT_UNREACHABLE();
}

/*
 * --------------------------------------------------------------------------
 * Runtime scheduler
 * --------------------------------------------------------------------------
 */

void
__gt_runtime_schedule(struct gt_task *prev)
{
    struct gt_task *next;

    /*
     * Re-admit the previously running task if it voluntarily yielded and
     * remains runnable.
     */
    if (prev && prev->t_state == GT_TASK_STATE_READY)
    {
        gt_scheduler_admit(&runtime.rt_scheduler, prev);
    }

    next = __gt_runtime_select_next();

    if (next == NULL)
    {
        __gt_runtime_resume_main();
    }

    /* cppcheck-suppress nullPointerRedundantCheck */
    __gt_runtime_resume_task(next);
}
