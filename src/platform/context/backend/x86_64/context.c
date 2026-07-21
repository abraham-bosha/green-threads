#include <gt_internal/platform/context/context.h>

#include <stddef.h>
#include <stdint.h>

#include <gt/error.h>
#include <gt_internal/assert/assert.h>
#include <gt_internal/common/align.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/mem.h>

/*
 * Architecture-specific bootstrap routine implemented in switch.S.
 *
 * A newly initialized context begins execution here the first time it
 * is resumed.
 */
extern GT_NORETURN void
gt_context_start(void);

static GT_FORCE_INLINE void
__gt_context_validate_context(const struct gt_context *GT_MAYBE_UNUSED context)
{
    GT_ASSERT(context != NULL);
}

static GT_FORCE_INLINE void
__gt_context_validate_entry(gt_context_entry_fn GT_MAYBE_UNUSED entry)
{
    GT_ASSERT(entry != NULL);
}

static GT_FORCE_INLINE void
__gt_context_validate_stack(void *GT_MAYBE_UNUSED stack_base, size_t GT_MAYBE_UNUSED stack_size)
{
    GT_ASSERT(stack_base != NULL);
    GT_ASSERT(stack_size != 0UL);

    GT_ASSERT(GT_IS_ALIGNED((uintptr_t)stack_base, GT_CONTEXT_STACK_ALIGNMENT));
    GT_ASSERT(GT_IS_ALIGNED(stack_size, GT_CONTEXT_STACK_ALIGNMENT));
}

static GT_FORCE_INLINE void
__gt_context_clear_context(struct gt_context *context)
{
    gt_mem_zero(context, sizeof(*context));
}

gt_status_t
gt_context_init(struct gt_context *context,
                gt_context_entry_fn entry,
                void *ctx_arg,
                void *stack_base,
                size_t stack_size)
{
    __gt_context_validate_context(context);
    __gt_context_validate_entry(entry);
    __gt_context_validate_stack(stack_base, stack_size);

    /*
     * Begin constructing the initial stack frame from the top of the
     * caller-provided stack region.
     */
    uintptr_t stack;
    uintptr_t *sp;

    stack = (uintptr_t)stack_base + stack_size;

    sp = (uintptr_t *)stack;

    /*
     * Initial stack layout:
     *             ctx_arg
     *             entry
     *      rsp -> gt_context_start
     *
     * The first context switch returns into gt_context_start(), which
     * invokes:
     *
     *      entry(ctx_arg)
     */
    *--sp = (uintptr_t)ctx_arg;
    *--sp = (uintptr_t)entry;
    *--sp = (uintptr_t)gt_context_start;

    __gt_context_clear_context(context);

    context->rsp = (uintptr_t)sp;

    return GT_STATUS_SUCCESS;
}

void
gt_context_destroy(struct gt_context *context)
{
    __gt_context_validate_context(context);

    __gt_context_clear_context(context);
}
