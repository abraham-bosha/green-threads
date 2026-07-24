/*
 * Unit tests for the platform context subsystem.
 *
 * Verifies context initialization, initial stack construction, and
 * context destruction. Context switching is tested separately once
 * the task subsystem is available.
 */

#include <gt_internal/platform/context/context.h>

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <gt_internal/common/compiler.h>

static int g_test_execution_flag = 0;

static void
mock_task_trampoline(void *arg)
{
    int *flag = (int *)arg;
    *flag = 1;
}

static void
test_context_initial_invariants(void)
{
    alignas(GT_CONTEXT_STACK_ALIGNMENT) unsigned long stack[1024];

    struct gt_context context;

    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_context_init(
        &context, mock_task_trampoline, &g_test_execution_flag, stack, sizeof(stack));

    assert(status == GT_STATUS_SUCCESS);
    assert(context.rsp != 0UL);

    assert(context.rbx == 0UL);
    assert(context.rbp == 0UL);
    assert(context.r12 == 0UL);
    assert(context.r13 == 0UL);
    assert(context.r14 == 0UL);
    assert(context.r15 == 0UL);

    uintptr_t GT_MAYBE_UNUSED stack_top = (uintptr_t)stack + sizeof(stack);
    assert(context.rsp < stack_top);
    assert(context.rsp >= (uintptr_t)stack);

    assert((context.rsp % GT_CONTEXT_STACK_ALIGNMENT) == sizeof(uintptr_t));

    gt_context_destroy(&context);
    assert(context.rsp == 0UL);
}

static void
test_context_stack_payload_order(void)
{
    alignas(GT_CONTEXT_STACK_ALIGNMENT) unsigned long stack[512];

    struct gt_context context;

    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_context_init(
        &context, mock_task_trampoline, &g_test_execution_flag, stack, sizeof(stack));

    assert(status == GT_STATUS_SUCCESS);
    assert(context.rsp != 0UL);

    const uintptr_t *GT_MAYBE_UNUSED sp = (uintptr_t *)context.rsp;

    assert(sp[0] != 0UL);
    assert(sp[1] == (uintptr_t)mock_task_trampoline);
    assert(sp[2] == (uintptr_t)&g_test_execution_flag);

    gt_context_destroy(&context);
    assert(context.rsp == 0UL);
}

static void
test_context_destroy_clears_context(void)
{
    alignas(GT_CONTEXT_STACK_ALIGNMENT) unsigned long stack[4096];

    struct gt_context context;

    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_context_init(
        &context, mock_task_trampoline, &g_test_execution_flag, stack, sizeof(stack));

    assert(status == GT_STATUS_SUCCESS);
    assert(context.rsp != 0UL);

    gt_context_destroy(&context);

    assert(context.rsp == 0UL);
    assert(context.rbx == 0UL);
    assert(context.rbp == 0UL);
    assert(context.r12 == 0UL);
    assert(context.r13 == 0UL);
    assert(context.r14 == 0UL);
    assert(context.r15 == 0UL);
}

int
main(void)
{
    puts("[RUN] platform/context");

    test_context_initial_invariants();
    test_context_stack_payload_order();
    test_context_destroy_clears_context();

    puts("[PASS] platform/context");
    return 0;
}
