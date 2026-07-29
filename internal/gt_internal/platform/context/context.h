#pragma once

/**
 * @file context.h
 * @brief Platform execution context abstraction.
 */

#include <stddef.h>
#include <stdint.h>

#include <gt/error.h>
#include <gt_internal/common/compiler.h>

/**
 * @brief Required alignment of every task stack.
 */
#define GT_CONTEXT_STACK_ALIGNMENT 16

/**
 * @brief Execution entry point of a newly created context.
 *
 * @param arg
 *      User-defined argument passed to the entry function.
 */
typedef void (*gt_context_entry_fn)(void *arg);

/**
 * @brief Saved execution context.
 *
 * Represents the processor state required to suspend and later resume
 * execution.
 */
struct gt_context
{
    uintptr_t rsp;

    uintptr_t rbx;
    uintptr_t rbp;

    uintptr_t r12;
    uintptr_t r13;
    uintptr_t r14;
    uintptr_t r15;
};

/**
 * @brief Initializes an execution context.
 *
 * Initializes the context object for use with the supplied stack.
 * This function does not configure an entry point.
 */
gt_status_t
gt_context_init(struct gt_context *ctx, void *stack_base, size_t stack_size);

/**
 * @brief Configures the initial execution state of a context.
 *
 * Prepares a previously initialized context so that its first execution
 * begins at the supplied entry point with the supplied argument.
 *
 * This function shall be called exactly once before the context is first
 * resumed.
 */
gt_status_t
gt_context_configure(struct gt_context *ctx, gt_context_entry_fn entry, void *arg);

/**
 * @brief Saves the currently executing context.
 *
 * Stores the processor state of the current execution context into the
 * supplied context object.
 *
 * @return
 *  - 0 when the context has just been saved.
 *  - Non-zero when the context is later resumed.
 */
int
gt_context_save(struct gt_context *current);

/**
 * @brief Restores a previously saved execution context.
 *
 * Restores the processor state stored in the supplied context. Execution
 * continues from the point at which that context was previously saved.
 *
 * This function does not return.
 */
GT_NORETURN
void
gt_context_load(const struct gt_context *next);

/**
 * @brief Releases resources associated with an execution context.
 */
void
gt_context_destroy(struct gt_context *ctx);
