#include <gt_internal/common/align.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

void
test_power_of_two_alignment_up(void)
{
    assert(GT_ALIGN_UP_POT(0, 16) == 0);
    assert(GT_ALIGN_UP_POT(4, 16) == 16);
    assert(GT_ALIGN_UP_POT(15, 16) == 16);

    assert(GT_ALIGN_UP_POT(1, 4096) == 4096);
    assert(GT_ALIGN_UP_POT(4095, 4096) == 4096);
    assert(GT_ALIGN_UP_POT(4097, 4096) == 8192);
}

void
test_power_of_two_alignment_down(void)
{
    assert(GT_ALIGN_DOWN_POT(0, 16) == 0);
    assert(GT_ALIGN_DOWN_POT(4, 16) == 0);
    assert(GT_ALIGN_DOWN_POT(15, 16) == 0);

    assert(GT_ALIGN_DOWN_POT(16, 16) == 16);
    assert(GT_ALIGN_DOWN_POT(19, 16) == 16);
    assert(GT_ALIGN_DOWN_POT(31, 16) == 16);
}

void
test_arbitrary_alignment_any(void)
{
    assert(GT_ALIGN_UP_ANY(0, 6) == 0);
    assert(GT_ALIGN_UP_ANY(4, 6) == 6);
    assert(GT_ALIGN_UP_ANY(6, 6) == 6);

    assert(GT_ALIGN_DOWN_ANY(4, 6) == 0);
    assert(GT_ALIGN_DOWN_ANY(6, 6) == 6);
    assert(GT_ALIGN_DOWN_ANY(11, 6) == 6);
}

void
test_alignment_predicate(void)
{
    assert(GT_IS_ALIGNED(0, 16) == true);
    assert(GT_IS_ALIGNED(32, 16) == true);

    assert(GT_IS_ALIGNED(4, 16) == false);
    assert(GT_IS_ALIGNED(17, 16) == false);
}

int
main(void)
{
    puts("[RUN] common/align");

    test_power_of_two_alignment_up();
    test_power_of_two_alignment_down();
    test_arbitrary_alignment_any();
    test_alignment_predicate();

    puts("[PASS] common/align");
    return 0;
}
