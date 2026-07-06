#include <gt/error.h>
#include <stdio.h>

int
main(void)
{
    gt_status_t status = GT_STATUS_INVALID_ARGUMENT;

    printf("status: %s\n", gt_status_string(status));

    return 0;
}
