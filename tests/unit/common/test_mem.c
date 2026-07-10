#include <gt_internal/common/mem.h>

#include <assert.h>
#include <stdio.h>

void
test_page_size(void)
{
    assert(GT_PAGE_SIZE == 4096);
}

int
main(void)
{
    puts("[RUN] common/mem");

    test_page_size();

    puts("[PASS] common/mem");

    return 0;
}
