#pragma once

#include <stddef.h>

#include <gt/error.h>

#include <gt_internal/platform/vm/vm.h>

/*
 * Stack alignment required by the runtime.
 */
#define GT_STACK_ALIGNMENT 16UL

/*
 * Number of guard pages per stack.
 */
#define GT_STACK_GUARD_PAGES 2UL

/*
 * Default usable stack size.
 */
#define GT_STACK_DEFAULT_SIZE (128 * 1024)

/*
 * Runtime execution stack.
 */
struct gt_stack
{
    struct gt_vm_mapping mapping;

    void *stack_base;
    size_t stack_size;
};

/*
 * Initializes an execution stack.
 *
 * On success, the stack owns a virtual memory mapping and exposes a
 * usable execution stack region.
 */
gt_status_t
gt_stack_init(struct gt_stack *s, size_t stack_size);

/*
 * Destroys an execution stack.
 *
 * Releases all resources owned by the stack and returns it to the
 * uninitialized state.
 */
void
gt_stack_destroy(struct gt_stack *s);
