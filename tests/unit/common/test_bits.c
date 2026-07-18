#include <gt_internal/common/bits.h>

#include <assert.h>
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
    puts("[RUN] common/bits");

    test_bits_and_word_widths();
    test_power_of_two_predicate();

    puts("[PASS] common/bits");

    return 0;
}
