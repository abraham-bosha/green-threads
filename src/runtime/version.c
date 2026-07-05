#include <gt/version.h>

#include <stdint.h>

/*
 * Runtime Version Metadata Queries
 *
 * Exposes immutable library metadata to external and internal execution loops.
 */

uint32_t gt_version_number(void)
{
    return GT_VERSION_NUMBER;
}

const char *gt_version_string(void)
{
    return GT_VERSION_STRING;
}
