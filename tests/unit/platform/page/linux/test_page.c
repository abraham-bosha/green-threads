#include <gt_internal/platform/page/page.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt_internal/common/bits.h>
#include <gt_internal/common/compiler.h>

static void
test_page_init(void)
{
    gt_status_t GT_MAYBE_UNUSED status = gt_page_init();
    assert(status == GT_STATUS_SUCCESS);

    size_t GT_MAYBE_UNUSED ps = gt_page_size();

    assert(ps > 0UL);
    assert(GT_IS_POW2(ps));
}

static void
test_page_address_alignment(void)
{
    size_t GT_MAYBE_UNUSED ps = gt_page_size();

    const void *GT_MAYBE_UNUSED aligned_addr1 = (const void *)0x1000UL;
    const void *GT_MAYBE_UNUSED aligned_addr2 = (const void *)(ps * 5UL);

    assert(gt_page_is_aligned(aligned_addr1) == true);
    assert(gt_page_is_aligned(aligned_addr2) == true);

    const void *GT_MAYBE_UNUSED unaligned_addr1 = (const void *)0x1005UL;
    const void *GT_MAYBE_UNUSED unaligned_addr2 = (const void *)((uintptr_t)aligned_addr2 + 1UL);

    assert(gt_page_is_aligned(unaligned_addr1) == false);
    assert(gt_page_is_aligned(unaligned_addr2) == false);
}

static void
test_page_size_alignment(void)
{
    size_t GT_MAYBE_UNUSED ps = gt_page_size();

    assert(gt_page_is_size_aligned(0UL) == true);
    assert(gt_page_is_size_aligned(ps) == true);
    assert(gt_page_is_size_aligned(ps * 4UL) == true);

    assert(gt_page_is_size_aligned(1UL) == false);
    assert(gt_page_is_size_aligned(ps - 1UL) == false);
    assert(gt_page_is_size_aligned(ps + 1UL) == false);
}

static void
test_page_align_up(void)
{
    size_t GT_MAYBE_UNUSED ps = gt_page_size();

    assert(gt_page_align_up(0UL) == 0UL);
    assert(gt_page_align_up(ps) == ps);
    assert(gt_page_align_up(ps * 2UL) == ps * 2UL);

    assert(gt_page_align_up(1UL) == ps);
    assert(gt_page_align_up(ps - 1UL) == ps);
    assert(gt_page_align_up(ps + 1UL) == ps * 2UL);
    assert(gt_page_align_up((ps * 3UL) - 50UL) == ps * 3UL);
}

int
main(void)
{
    puts("[RUN] platform/page");

    test_page_init();
    test_page_address_alignment();
    test_page_size_alignment();
    test_page_align_up();

    puts("[PASS] platform/page");
    return 0;
}
