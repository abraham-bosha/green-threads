#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * @brief Unique identifier assigned to a task.
 */
typedef size_t gt_task_id_t;

/**
 * @brief Runtime lifecycle states.
 */
typedef enum
{
    GT_RUNTIME_STATE_UNINITIALIZED = 0, /* Runtime has not been initialized. */
    GT_RUNTIME_STATE_INITIALIZED,       /* Runtime is initialized and ready. */
    GT_RUNTIME_STATE_RUNNING,           /* Scheduler is actively executing tasks. */
    GT_RUNTIME_STATE_STOPPED            /* Runtime has terminated. */
} gt_runtime_state_t;

/**
 * @brief Task execution lifecycle states.
 */
typedef enum
{
    GT_TASK_STATE_NEW = 0, /* Created but not yet scheduled. */
    GT_TASK_STATE_READY,   /* Ready to execute. */
    GT_TASK_STATE_RUNNING, /* Currently executing. */
    GT_TASK_STATE_ZOMBIE   /* Finished execution. */
} gt_task_state_t;

/**
 * @brief Entry function executed by a task.
 */
typedef void (*gt_task_entry_fn)(void *arg);

/*
 * Forward Declarations
 */

/* Runtime objects */
struct gt_runtime;
struct gt_task;
struct gt_stack;
struct gt_context;
struct gt_vm_mapping;

/* Internal data structures */
struct gt_list_node;
struct gt_list;
struct gt_queue;
struct gt_bitmap;
struct gt_id_pool;
