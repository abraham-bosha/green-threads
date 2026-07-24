#include <gt_internal/memory/stack/stack.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/align.h>
#include <gt_internal/platform/page/page.h>
#include <gt_internal/common/compiler.h>

static void
test_stack_initial_invariants(void)
{
    struct gt_stack stack;
    size_t stack_size;
    size_t ps;

    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_page_init();
    assert(status == GT_STATUS_SUCCESS);

    ps = gt_page_size();
    stack_size = (4UL * ps) - 700UL;
    status = gt_stack_init(&stack, stack_size);

    assert(status == GT_STATUS_SUCCESS);
    assert(stack.stack_base != NULL);
    assert(stack.stack_size != 0UL);

    assert(GT_IS_ALIGNED(stack.stack_size, ps));
    assert(GT_IS_ALIGNED((uintptr_t)stack.stack_base, GT_STACK_ALIGNMENT));
    assert(GT_IS_ALIGNED(stack.stack_size, GT_STACK_ALIGNMENT));

    assert((uintptr_t)stack.stack_base > (uintptr_t)stack.mapping.vm_base);
    assert(stack.stack_size < stack.mapping.vm_size);

    volatile int *ptr = (volatile int *)stack.stack_base;
    *ptr = 0xDEADBEEF;

    assert(*ptr == (volatile int)0xDEADBEEF);

    gt_stack_destroy(&stack);
    
    assert(stack.mapping.vm_base == NULL);
    assert(stack.mapping.vm_size == 0UL);

    assert(stack.stack_base == NULL);
    assert(stack.stack_size == 0UL);
}

static void
test_stack_init_with_size_zero(void)
{
    struct gt_stack stack;
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_stack_init(&stack, 0UL);

    assert(status == GT_STATUS_SUCCESS);
    assert(stack.stack_size == GT_STACK_DEFAULT_SIZE);
    assert(GT_IS_ALIGNED(stack.stack_size, gt_page_size()));
    assert(stack.stack_base != NULL);

    gt_stack_destroy(&stack);

    assert(stack.mapping.vm_base == NULL);
    assert(stack.mapping.vm_size == 0UL);

    assert(stack.stack_base == NULL);
    assert(stack.stack_size == 0UL);
}

int
main(void)
{
    puts("[RUN] memory/stack");

    test_stack_initial_invariants();
    test_stack_init_with_size_zero();

    puts("[PASS] memory/stack");
    return 0;
}
