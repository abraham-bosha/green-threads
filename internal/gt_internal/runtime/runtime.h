#pragma once

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/common/types.h>

/*
 * Runtime lifecycle.
 */

gt_status_t
gt_runtime_init(void);

/*
 * Creates a new task owned by the runtime.
 */
gt_status_t
gt_runtime_create_task(gt_task_entry_fn entry, void *arg, size_t stack_size);

/*
 * Starts cooperative task execution.
 */
void
gt_runtime_run(void);

/*
 * Voluntarily yields execution to another runnable task.
 */
void
gt_runtime_yield(void);

/*
 * Releases every runtime resource.
 */
void
gt_runtime_destroy(void);
