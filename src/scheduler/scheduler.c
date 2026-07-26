#include <gt_internal/scheduler/scheduler.h>

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/common/mem.h>
#include <gt_internal/task/task.h>

static GT_FORCE_INLINE void
__gt_scheduler_validate_sched(const struct gt_scheduler *GT_MAYBE_UNUSED sched)
{
    GT_ASSERT(sched != NULL);
}

static GT_FORCE_INLINE void
__gt_scheduler_validate_task(const struct gt_task *GT_MAYBE_UNUSED task)
{
    GT_ASSERT(task != NULL);
}

static GT_FORCE_INLINE void
__gt_scheduler_clear(struct gt_scheduler *sched)
{
    gt_mem_clear(sched, sizeof(*sched));
}

static GT_FORCE_INLINE void
__gt_scheduler_validate_task_state(const struct gt_task *GT_MAYBE_UNUSED task)
{
    GT_ASSERT(task->t_state == GT_TASK_STATE_READY);
}

gt_status_t
gt_scheduler_init(struct gt_scheduler *sched)
{
    __gt_scheduler_validate_sched(sched);
    __gt_scheduler_clear(sched);

    gt_queue_init(&sched->ready_queue);

    return GT_STATUS_SUCCESS;
}

void
gt_scheduler_destroy(struct gt_scheduler *sched)
{
    __gt_scheduler_validate_sched(sched);

    __gt_scheduler_clear(sched);
}

gt_status_t
gt_scheduler_admit(struct gt_scheduler *sched, struct gt_task *task)
{
    __gt_scheduler_validate_sched(sched);
    __gt_scheduler_validate_task(task);
    __gt_scheduler_validate_task_state(task);

    gt_queue_enqueue(&sched->ready_queue, &task->t_ready_node);

    return GT_STATUS_SUCCESS;
}

struct gt_task *
gt_scheduler_next(struct gt_scheduler *sched)
{
    __gt_scheduler_validate_sched(sched);

    struct gt_list_node *ready_node = gt_queue_dequeue(&sched->ready_queue);

    return GT_CONTAINER_OF_SAFE(ready_node, struct gt_task, t_ready_node);
}
