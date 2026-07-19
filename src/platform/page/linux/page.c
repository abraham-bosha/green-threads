/**
 * @file page.c
 * @brief Linux implementation of the platform page abstraction.
 */

#include <gt_internal/platform/page/page.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

#include <gt/error.h>

#include <gt_internal/common/align.h>
#include <gt_internal/common/bits.h>
#include <gt_internal/common/compiler.h>

/* Cached system page size discovered during platform initialization. */
static size_t __gt_page_size;

static bool __gt_page_size_initialized = false;

/* Largest value that can be safely aligned upward without overflow. */
static size_t __gt_page_max_alignable_size;

/* -------------------------------------------------------------------------- */
/* Internal Validation Helpers                                                */
/* -------------------------------------------------------------------------- */

static GT_FORCE_INLINE void
__gt_page_validate_page_size(void)
{
    assert(GT_IS_POW2(__gt_page_size));
}

static GT_FORCE_INLINE void
__gt_page_validate_initialized(void)
{
    assert(__gt_page_size_initialized);
}

static GT_FORCE_INLINE void
__gt_page_validate_address(const void *GT_MAYBE_UNUSED address)
{
    assert(address != NULL);
}

static GT_FORCE_INLINE void
__gt_page_validate_alignable_size(size_t GT_MAYBE_UNUSED size)
{
    assert(size <= __gt_page_max_alignable_size);
}

/* -------------------------------------------------------------------------- */
/* Public API Implementations                                                 */
/* -------------------------------------------------------------------------- */

gt_status_t
gt_page_init(void)
{
    // assert(!__gt_page_size_initialized);

    /* Discover the system page size from the operating system. */
    long ps = sysconf(_SC_PAGE_SIZE);

    if (ps <= 0)
    {
        return GT_STATUS_PAGE_SIZE_UNAVAILABLE;
    }

    __gt_page_size = (size_t)ps;

    /* Ensure the reported page size satisfies runtime assumptions. */
    __gt_page_validate_page_size();

    /* Compute the largest value that can be safely page-aligned upward. */
    __gt_page_max_alignable_size = GT_ALIGN_DOWN_ANY(SIZE_MAX, __gt_page_size);

    __gt_page_size_initialized = true;

    return GT_STATUS_SUCCESS;
}

size_t
gt_page_size(void)
{
    __gt_page_validate_initialized();
    return __gt_page_size;
}

bool
gt_page_is_aligned(const void *address)
{
    __gt_page_validate_initialized();
    __gt_page_validate_address(address);

    return GT_IS_ALIGNED((uintptr_t)address, __gt_page_size);
}

bool
gt_page_is_size_aligned(size_t size)
{
    __gt_page_validate_initialized();

    return GT_IS_ALIGNED(size, __gt_page_size);
}

size_t
gt_page_align_up(size_t size)
{
    __gt_page_validate_initialized();
    __gt_page_validate_alignable_size(size);

    return GT_ALIGN_UP_ANY(size, __gt_page_size);
}
