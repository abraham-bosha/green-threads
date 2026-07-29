#pragma once

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/common/types.h>

#include <gt_internal/ds/list/list.h>
#include <gt_internal/memory/stack/stack.h>
#include <gt_internal/platform/context/context.h>

/**
 * @brief Runtime task object.
 *
 * A task owns all resources required to execute a single unit of work.
 */
struct gt_task
{
    /**
     * @brief Runtime-assigned task identifier.
     */
    gt_task_id_t t_id;

    /**
     * @brief Current execution state.
     */
    gt_task_state_t t_state;

    /**
     * @brief User entry function.
     */
    gt_task_entry_fn t_entry;

    /**
     * @brief User argument passed to the entry function.
     */
    void *t_arg;

    /**
     * @brief Execution stack owned by the task.
     */
    struct gt_stack t_stack;

    /**
     * @brief Execution context owned by the task.
     */
    struct gt_context t_context;

    /*
     * @brief Intrusive list node for membership in the scheduler's ready queue.
     */
    struct gt_list_node t_ready_node;

    /*
     * @brief Intrusive list node for membership in the runtime's task list.
     */
    struct gt_list_node t_runtime_node;
};

/**
 * @brief Initializes a task.
 *
 * Constructs an executable task using the supplied identifier, entry
 * function, argument, and stack size.
 *
 * @param t          Task to initialize.
 * @param id         Runtime-assigned task identifier.
 * @param entry      Task entry function.
 * @param arg        Argument passed to the entry function.
 * @param stack_size Requested execution stack size in bytes. A value of
 *                   zero selects the default stack size.
 *
 * @retval GT_STATUS_SUCCESS Initialization succeeded.
 * @retval Other             Initialization failed.
 */
gt_status_t
gt_task_init(
    struct gt_task *task, gt_task_id_t id, gt_task_entry_fn entry, void *arg, size_t stack_size);

/**
 * @brief Destroys a task.
 *
 * Releases all resources owned by the task and returns it to the
 * uninitialized state.
 *
 * @param task Task to destroy.
 */
void
gt_task_destroy(struct gt_task *task);
