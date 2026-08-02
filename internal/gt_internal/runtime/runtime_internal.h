#pragma once

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/common/align.h>
#include <gt_internal/common/bits.h>
#include <gt_internal/common/types.h>
#include <gt_internal/ds/id_pool/id_pool.h>
#include <gt_internal/ds/list/list.h>
#include <gt_internal/platform/context/context.h>
#include <gt_internal/scheduler/scheduler.h>

#define GT_TASK_MAX_CAPACITY 100000

#define GT_RUNTIME_TASK_ID_STORAGE \
    (GT_ALIGN_UP_POT(GT_TASK_MAX_CAPACITY, GT_BITS_PER_WORD) / GT_BITS_PER_WORD)

/*
 * Runtime state shared by every runtime implementation source file.
 */
struct gt_runtime
{
    /*
     * Current runtime lifecycle state.
     */
    gt_runtime_state_t rt_state;

    /*
     * Scheduler responsible for selecting runnable tasks.
     */
    struct gt_scheduler rt_scheduler;

    /*
     * Execution context of the thread that entered gt_run().
     */
    struct gt_context rt_main_context;

    /*
     * Bitmap storage backing the task ID pool.
     */
    unsigned long rt_task_id_storage[GT_RUNTIME_TASK_ID_STORAGE];

    /*
     * Task identifier allocator.
     */
    struct gt_id_pool rt_task_ids;

    /*
     * Intrusive list containing every task owned by the runtime.
     */
    struct gt_list rt_tasks;

    struct gt_list rt_zombies;

    /*
     * Number of tasks currently owned by the runtime.
     */
    size_t rt_task_count;

    /*
     * Currently executing task.
     *
     * NULL while executing in the main context.
     */
    struct gt_task *rt_current;
};

/*
 * Singleton runtime instance.
 */
extern struct gt_runtime runtime;

/*
 * Runtime scheduling routine.
 */
void
__gt_runtime_schedule(struct gt_task *prev);

/*
 * Allocates and initializes a runtime-owned task.
 */
gt_status_t
__gt_runtime_task_create(gt_task_entry_fn entry, void *arg, size_t stack_size);

/*
 * Destroys a runtime-owned task.
 */
void
__gt_runtime_task_destroy(struct gt_task *task);

/*
 * Entry point executed when a task runs for the first time.
 */
GT_NORETURN void
__gt_runtime_task_trampoline(void *arg);

/*
 * Handles task completion after the task entry function returns.
 */
GT_NORETURN void
__gt_runtime_task_exit(void);

void
__gt_runtime_destroy_task_list(struct gt_list *list);

void
__gt_runtime_reap_zombies(void);
