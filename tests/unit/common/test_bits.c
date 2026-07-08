#include <assert.h>
#include <gt_internal/common/bits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

void
test_bits_and_word_widths(void)
{
    assert(GT_BITS_PER_BYTE == 8);
    assert(GT_BITS_PER_WORD == 64);
}

void
test_bit64_mask_generation(void)
{
    assert(GT_BIT64(0) == 0x1ULL);
    assert(GT_BIT64(1) == 0x2ULL);
    assert(GT_BIT64(3) == 0x8ULL);
    assert(GT_BIT64(63) == 0x8000000000000000ULL);
}

void
test_power_of_two_predicate(void)
{
    assert(GT_IS_POW2(16) == true);
    assert(GT_IS_POW2(128) == true);
    assert(GT_IS_POW2(4096) == true);

    assert(GT_IS_POW2(17) == false);
    assert(GT_IS_POW2(129) == false);
    assert(GT_IS_POW2(4000) == false);
}

int
main(void)
{
    printf("[RUN] common/bits\n");

    test_bits_and_word_widths();
    test_bit64_mask_generation();
    test_power_of_two_predicate();

    printf("[PASSED] common/bits\n");

    return 0;
}
