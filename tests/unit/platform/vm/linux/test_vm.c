#include <gt_internal/platform/vm/vm.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/platform/page/page.h>

static void
gt_test_vm_init_dependencies(void)
{
    gt_status_t GT_MAYBE_UNUSED status = gt_page_init();

    assert(status == GT_STATUS_SUCCESS);
}

static void
test_vm_mapping_reserve_and_release(void)
{
    gt_test_vm_init_dependencies();
    size_t ps = gt_page_size();

    struct gt_vm_mapping m;

    size_t request_size = ps * 2UL;

    gt_status_t GT_MAYBE_UNUSED status = gt_vm_mapping_reserve(&m, request_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(m.base != NULL);
    assert(m.size == request_size);
    assert(gt_page_is_aligned(m.base) == true);

    status = gt_vm_mapping_release(&m);
    assert(status == GT_STATUS_SUCCESS);

    assert(m.base == NULL);
    assert(m.size == 0UL);
}

static void
test_vm_mapping_protection_transitions(void)
{
    size_t ps = gt_page_size();
    struct gt_vm_mapping m;

    gt_status_t GT_MAYBE_UNUSED status = gt_vm_mapping_reserve(&m, ps);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_vm_mapping_protect(&m, GT_VM_ACCESS_READ_WRITE);
    assert(status == GT_STATUS_SUCCESS);

    volatile int *ptr = (volatile int *)m.base;
    *ptr = 0xDEADBEEF;
    assert(*ptr == (int)0xDEADBEEF);

    status = gt_vm_mapping_protect(&m, GT_VM_ACCESS_NONE);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_vm_mapping_release(&m);
    assert(status == GT_STATUS_SUCCESS);
}

static void
test_vm_mapping_allocation_limits(void)
{
    gt_test_vm_init_dependencies();
    size_t ps = gt_page_size();

    struct gt_vm_mapping m;

    size_t impossible_size = gt_page_align_up(INTPTR_MAX - ps);

    gt_status_t GT_MAYBE_UNUSED status = gt_vm_mapping_reserve(&m, impossible_size);

    assert(status == GT_STATUS_OUT_OF_MEMORY);
    assert(m.base == NULL);
    assert(m.size == 0UL);
}

int
main(void)
{
    puts("[RUN] platform/vm");

    test_vm_mapping_reserve_and_release();
    test_vm_mapping_protection_transitions();
    test_vm_mapping_allocation_limits();

    puts("[PASS] platform/vm");
    return 0;
}
