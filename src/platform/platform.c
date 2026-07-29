#include <gt_internal/platform/platform.h>

#include <gt_internal/assert/assert.h>

gt_status_t
gt_platform_init(void)
{
    gt_status_t status;

    status = gt_page_init();

    if (status != GT_STATUS_SUCCESS)
    {
        return status;
    }

    return GT_STATUS_SUCCESS;
}

void
gt_platform_destroy(void)
{
    return;
}
