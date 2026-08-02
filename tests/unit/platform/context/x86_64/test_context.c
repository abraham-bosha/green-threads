#include <gt_internal/platform/context/context.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/align.h>
#include <gt_internal/common/compiler.h>

extern GT_NORETURN void
gt_context_start(void);

static volatile int g_test_execution_flag = 0;

/**
 * @brief Standard mock entry function matching your gt_context_entry_fn signature.
 */
static void
mock_task_trampoline(void *arg)
{
    int *flag = (int *)arg;
    *flag = 1;
}

/**
 * @brief Validates baseline structure zero-flushing and stack bounding math.
 */
static void
test_context_initialization_invariants(void)
{
    uintptr_t static_stack[512] GT_ALIGNED(GT_CONTEXT_STACK_ALIGNMENT);

    struct gt_context ctx;

    gt_status_t GT_MAYBE_UNUSED status =
        gt_context_init(&ctx, (void *)static_stack, sizeof(static_stack));
    assert(status == GT_STATUS_SUCCESS);

    uintptr_t GT_MAYBE_UNUSED stack_top_boundary = (uintptr_t)static_stack + sizeof(static_stack);
    assert(ctx.rsp == stack_top_boundary);
    assert(GT_IS_ALIGNED(ctx.rsp, GT_CONTEXT_STACK_ALIGNMENT));

    assert(ctx.rbx == 0UL);
    assert(ctx.rbp == 0UL);
    assert(ctx.r12 == 0UL);
    assert(ctx.r13 == 0UL);
    assert(ctx.r14 == 0UL);
    assert(ctx.r15 == 0UL);

    gt_context_destroy(&ctx);
    assert(ctx.rsp == 0UL);
}

/**
 * @brief Validates the asymmetric stack frame carving pushing order.
 */
static void
test_context_configuration_payload_order(void)
{
    uintptr_t static_stack[512] GT_ALIGNED(GT_CONTEXT_STACK_ALIGNMENT);

    struct gt_context ctx;
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_context_init(&ctx, (void *)static_stack, sizeof(static_stack));
    assert(status == GT_STATUS_SUCCESS);

    status = gt_context_configure(&ctx, mock_task_trampoline, (void *)&g_test_execution_flag);
    assert(status == GT_STATUS_SUCCESS);

    const uintptr_t *GT_MAYBE_UNUSED sp = (uintptr_t *)ctx.rsp;

    assert(sp[0] == (uintptr_t)gt_context_start);
    assert(sp[1] == (uintptr_t)mock_task_trampoline);
    assert(sp[2] == (uintptr_t)&g_test_execution_flag);

    gt_context_destroy(&ctx);
}

int
main(void)
{
    puts("[RUN] platform/context");

    test_context_initialization_invariants();
    test_context_configuration_payload_order();

    puts("[PASS] platform/context");
    return 0;
}
