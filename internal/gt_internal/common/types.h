#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Unique identifier assigned to a task.
 */
typedef uint32_t gt_task_id_t;

/**
 * @brief Runtime lifecycle states.
 */
typedef enum
{
    GT_RUNTIME_UNINITIALIZED = 0, /* Runtime has not been initialized. */
    GT_RUNTIME_INITIALIZED,       /* Runtime is initialized and ready. */
    GT_RUNTIME_RUNNING,           /* Scheduler is actively executing tasks. */
    GT_RUNTIME_STOPPING,          /* Runtime is shutting down. */
    GT_RUNTIME_STOPPED            /* Runtime has terminated. */
} gt_runtime_state_t;

/**
 * @brief Task execution lifecycle states.
 */
typedef enum
{
    GT_TASK_STATE_CREATED = 0, /* Created but not yet scheduled. */
    GT_TASK_STATE_READY,       /* Ready to execute. */
    GT_TASK_STATE_RUNNING,     /* Currently executing. */
    GT_TASK_STATE_COMPLETED    /* Finished execution. */
} gt_task_state_t;

/**
 * @brief Entry function executed by a task.
 */
typedef void (*gt_task_entry_fn)(void *arg);

/*
 * Forward Declarations
 */

/* Runtime objects */
struct gt_vm_mapping;
struct gt_context;

/* Internal data structures */
struct gt_list_node;
struct gt_list;
struct gt_queue;
struct gt_bitmap;
struct gt_id_pool;
