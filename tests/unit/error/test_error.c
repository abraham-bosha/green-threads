#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <gt/error.h>

static void
test_compile_time_status_values(void)
{
    assert(GT_STATUS_SUCCESS == 0);
    assert(GT_STATUS_INVALID_ARGUMENT == 1);
    assert(GT_STATUS_INVALID_STATE == 2);
    assert(GT_STATUS_OUT_OF_MEMORY == 3);
}

static void
test_status_string_mapping(void)
{
    assert(strcmp(gt_status_string(GT_STATUS_SUCCESS), "success") == 0);
    assert(strcmp(gt_status_string(GT_STATUS_INVALID_ARGUMENT), "invalid argument") == 0);
    assert(strcmp(gt_status_string(GT_STATUS_INVALID_STATE), "invalid state") == 0);
    assert(strcmp(gt_status_string(GT_STATUS_OUT_OF_MEMORY), "out of memory") == 0);
    assert(strcmp(gt_status_string(GT_STATUS_INTERNAL_ERROR), "internal error") == 0);

    /* Default case */
    assert(strcmp(gt_status_string((gt_status_t)9999), "unknown status") == 0);
}

int
main(void)
{
    printf("[RUN] runtime/error\n");

    test_compile_time_status_values();
    test_status_string_mapping();

    printf("[PASSED] runtime/error\n");
    return 0;
}
