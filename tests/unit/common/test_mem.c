#include <assert.h>
#include <gt_internal/common/mem.h>
#include <stdio.h>

void
test_page_size(void)
{
    assert(GT_PAGE_SIZE == 4096);
}

int
main(void)
{
    printf("[RUN] common/mem\n");

    test_page_size();

    printf("[PASSED] common/mem\n");

    return 0;
}
