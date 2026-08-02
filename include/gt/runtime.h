#pragma once

#include <stddef.h>

#include <gt/error.h>

typedef void (*gt_task_entry_fn)(void *arg);

/*
 * Runtime lifecycle.
 */

gt_status_t
gt_init(void);

void
gt_destroy(void);

/*
 * Task management.
 */

gt_status_t
gt_create_task(gt_task_entry_fn entry, void *arg, size_t stack_size);

/*
 * Runtime execution.
 */

void
gt_run(void);

void
gt_yield(void);
