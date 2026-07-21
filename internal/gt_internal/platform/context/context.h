#pragma once

/**
 * @file context.h
 * @brief Platform execution context abstraction.
 */

#include <stddef.h>
#include <stdint.h>

#include <gt/error.h>

/**
 * @brief Required alignment of the initial task stack.
 *
 * Every execution context must begin with a stack pointer aligned
 * according to the target platform ABI.
 */
#define GT_CONTEXT_STACK_ALIGNMENT 16

/**
 * @brief Execution entry point.
 *
 * @param ctx_arg
 *      User-supplied argument passed to the entry function.
 */
typedef void (*gt_context_entry_fn)(void *ctx_arg);

/**
 * @brief Saved execution context.
 *
 * Represents the processor state required to suspend and later resume
 * execution.
 *
 * Ownership of the structure belongs exclusively to the Platform Context
 * subsystem. Runtime code may embed and pass instances of this type but
 * shall not inspect or modify its fields directly.
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
 * Prepares an execution context that begins execution at the supplied
 * entry point using the supplied stack.
 *
 * @param context
 *      Context to initialize.
 *
 * @param entry
 *      Initial execution entry point.
 *
 * @param ctx_arg
 *      Argument supplied to the entry point.
 *
 * @param stack_base
 *      Lowest address of the stack allocation.
 *
 * @param stack_size
 *      Stack allocation size in bytes.
 *
 * @return
 *      Operation status.
 */
gt_status_t
gt_context_init(struct gt_context *context,
                gt_context_entry_fn entry,
                void *ctx_arg,
                void *stack_base,
                size_t stack_size
            );

/**
 * @brief Saves the current execution context and resumes another.
 *
 * Execution continues in the supplied context until another context switch
 * restores the current one.
 *
 * @param current
 *      Currently executing context.
 *
 * @param next
 *      Context to resume.
 */
void
gt_context_switch(struct gt_context *current, const struct gt_context *next);

/**
 * @brief Releases resources associated with an execution context.
 *
 * @param context
 *      Context to destroy.
 *
 * @return
 *      Operation status.
 */
void
gt_context_destroy(struct gt_context *context);
