#pragma once

#include <gt/error.h>

#include <gt_internal/common/types.h>
#include <gt_internal/ds/queue/queue.h>

struct gt_scheduler
{
    /*
     * Runnable tasks ordered according to the active scheduling policy.
     */
    struct gt_queue ready_queue;
};

/**
 * @brief Initializes a scheduler.
 *
 * @param sched Scheduler to initialize.
 *
 * @return
 *      - GT_STATUS_SUCCESS on success.
 */
gt_status_t
gt_scheduler_init(struct gt_scheduler *sched);

/**
 * @brief Destroys a scheduler.
 *
 * @param sched Scheduler to destroy.
 */
void
gt_scheduler_destroy(struct gt_scheduler *sched);

/**
 * @brief Admits a runnable task into the scheduler.
 *
 * The task shall already be in the READY state.
 *
 * @param sched Target scheduler.
 * @param task  Runnable task.
 *
 * @return
 *      - GT_STATUS_SUCCESS on success.
 */
gt_status_t
gt_scheduler_admit(struct gt_scheduler *sched, struct gt_task *task);

/**
 * @brief Selects the next runnable task.
 *
 * Removes and returns the next task according to the scheduling policy.
 *
 * @param sched Scheduler instance.
 *
 * @return
 *      - Pointer to the selected task.
 *      - NULL if no runnable task exists.
 */
struct gt_task *
gt_scheduler_next(struct gt_scheduler *sched);
