#include <gt_internal/platform/context/context.h>

#include <stddef.h>
#include <stdint.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/align.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/mem.h>

/*
 * Architecture-specific bootstrap routine executed the first time a
 * newly configured execution context is resumed.
 */
extern GT_NORETURN void
gt_context_start(void);

/*
 * Architecture-specific context switching routines implemented in
 * switch.S.
 */
extern int
__gt_context_save_asm(struct gt_context *current);

extern GT_NORETURN void
__gt_context_load_asm(const struct gt_context *next);

/*
 * --------------------------------------------------------------------------
 * Validation helpers
 * --------------------------------------------------------------------------
 */

static GT_FORCE_INLINE void
__gt_context_validate_context(const struct gt_context *GT_MAYBE_UNUSED ctx)
{
    GT_ASSERT(ctx != NULL);
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
    GT_ASSERT(stack_size != 0);

    GT_ASSERT(GT_IS_ALIGNED((uintptr_t)stack_base, GT_CONTEXT_STACK_ALIGNMENT));
    GT_ASSERT(GT_IS_ALIGNED(stack_size, GT_CONTEXT_STACK_ALIGNMENT));
}

/*
 * --------------------------------------------------------------------------
 * Internal helpers
 * --------------------------------------------------------------------------
 */

static GT_FORCE_INLINE void
__gt_context_clear(struct gt_context *ctx)
{
    gt_mem_clear(ctx, sizeof(*ctx));
}

static GT_FORCE_INLINE void
__gt_context_stack_push(uintptr_t **sp, uintptr_t value)
{
    *--(*sp) = value;
}

static void
__gt_context_build_initial_frame(struct gt_context *ctx, gt_context_entry_fn entry, void *arg)
{
    GT_ASSERT(ctx->rsp != 0);

    uintptr_t *sp = (uintptr_t *)ctx->rsp;

    /*
     * Arrange the initial stack frame so that the first restoration of
     * this context resumes execution at gt_context_start(), which in
     * turn invokes:
     *
     *      entry(arg)
     */
    __gt_context_stack_push(&sp, (uintptr_t)arg);
    __gt_context_stack_push(&sp, (uintptr_t)entry);
    __gt_context_stack_push(&sp, (uintptr_t)gt_context_start);

    ctx->rsp = (uintptr_t)sp;
}

/*
 * --------------------------------------------------------------------------
 * Public API
 * --------------------------------------------------------------------------
 */

gt_status_t
gt_context_init(struct gt_context *ctx, void *stack_base, size_t stack_size)
{
    __gt_context_validate_context(ctx);
    __gt_context_validate_stack(stack_base, stack_size);

    uintptr_t stack;

    /*
     * Begin constructing the initial execution frame from the top of
     * the supplied stack region.
     */
    stack = (uintptr_t)stack_base + stack_size;

    stack = GT_ALIGN_DOWN_POT(stack, GT_CONTEXT_STACK_ALIGNMENT);

    __gt_context_clear(ctx);

    ctx->rsp = stack;

    return GT_STATUS_SUCCESS;
}

gt_status_t
gt_context_configure(struct gt_context *ctx, gt_context_entry_fn entry, void *arg)
{
    __gt_context_validate_context(ctx);
    __gt_context_validate_entry(entry);

    __gt_context_build_initial_frame(ctx, entry, arg);

    return GT_STATUS_SUCCESS;
}

int
gt_context_save(struct gt_context *current)
{
    __gt_context_validate_context(current);

    return __gt_context_save_asm(current);
}

GT_NORETURN
void
gt_context_load(const struct gt_context *next)
{
    __gt_context_validate_context(next);

    __gt_context_load_asm(next);

    GT_UNREACHABLE();
}

void
gt_context_destroy(struct gt_context *ctx)
{
    __gt_context_validate_context(ctx);

    __gt_context_clear(ctx);
}
