#include <gt_internal/runtime/runtime_internal.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/platform/context/context.h>
#include <gt_internal/task/task.h>

/*
 * --------------------------------------------------------------------------
 * Validation helpers
 * --------------------------------------------------------------------------
 */

static GT_FORCE_INLINE void
__gt_runtime_validate_current_task(void)
{
    GT_ASSERT(runtime.rt_current != NULL);
}

/*
 * --------------------------------------------------------------------------
 * Internal execution helpers
 * --------------------------------------------------------------------------
 */

/*
 * Terminates the currently executing task.
 *
 * This routine never returns.
 */
GT_NORETURN
void
__gt_runtime_task_exit(void)
{
    struct gt_task *current;

    __gt_runtime_validate_current_task();

    current = runtime.rt_current;

    current->t_state = GT_TASK_STATE_ZOMBIE;

    gt_list_remove(&current->t_runtime_node);

    gt_list_push_back(&runtime.rt_zombies, &current->t_runtime_node);

    runtime.rt_current = NULL;

    /*
     * Select another runnable task.
     *
     * The exited task must never be re-admitted to the ready queue.
     */
    __gt_runtime_schedule(NULL);

    GT_UNREACHABLE();
}

/*
 * Initial execution routine for every newly created task.
 *
 * Invokes the user entry function and automatically terminates the task
 * when the entry function returns.
 */
GT_NORETURN void
__gt_runtime_task_trampoline(void *arg)
{
    GT_ASSERT(arg != NULL);

    struct gt_task *task;

    task = (struct gt_task *)arg;

    /* cppcheck-suppress nullPointerRedundantCheck */
    task->t_entry(task->t_arg);

    __gt_runtime_task_exit();

    GT_UNREACHABLE();
}

/*
 * --------------------------------------------------------------------------
 * Public execution API
 * --------------------------------------------------------------------------
 */

void
gt_runtime_run(void)
{
    GT_ASSERT(runtime.rt_state == GT_RUNTIME_STATE_INITIALIZED ||
              runtime.rt_state == GT_RUNTIME_STATE_STOPPED);

    /*
     * Save the execution context of the calling thread.
     *
     * When every runnable task completes, the scheduler restores this
     * context, causing gt_context_save() to return non-zero.
     */
    if (gt_context_save(&runtime.rt_main_context) == 0)
    {
        runtime.rt_state = GT_RUNTIME_STATE_RUNNING;

        __gt_runtime_schedule(NULL);
    }

    __gt_runtime_reap_zombies();
}

void
gt_runtime_yield(void)
{
    struct gt_task *current;

    __gt_runtime_validate_current_task();

    current = runtime.rt_current;

    current->t_state = GT_TASK_STATE_READY;

    /*
     * Save the current task context.
     *
     * The first return schedules another runnable task.
     *
     * When this task is resumed later, gt_context_save() returns
     * non-zero and execution continues immediately after this call.
     */
    if (gt_context_save(&current->t_context) == 0)
    {
        __gt_runtime_schedule(current);
    }
}
