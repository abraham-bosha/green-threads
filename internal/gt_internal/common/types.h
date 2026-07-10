#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @brief Unique identifier assigned to a task.
 */
typedef uint32_t gt_task_id_t;

/**
 * @brief Integer type capable of storing a virtual memory address.
 */
typedef uintptr_t gt_addr_t;

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
 * @brief Supported context-switch backend implementations.
 */
typedef enum
{
    GT_BACKEND_UCONTEXT = 0, /* POSIX ucontext backend. */
    GT_BACKEND_SETJMP,       /* setjmp/longjmp backend. */
    GT_BACKEND_ASM,          /* Assembly backend. */
} gt_backend_type_t;

/**
 * @brief Entry function executed by a task.
 */
typedef void (*gt_task_entry_fn)(void *arg);

/*
 * Forward Declarations
 */

/* Core runtime objects */
struct gt_runtime;
struct gt_scheduler;
struct gt_task;
struct gt_context;

/* Memory subsystem */
struct gt_stack;
struct gt_memory;

/* Internal data structures */
struct gt_bitmap;
struct gt_run_queue;
struct gt_id_pool;
