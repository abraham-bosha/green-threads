#include <gt_internal/platform/heap/heap.h>

#include <stdlib.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>

static GT_FORCE_INLINE void
__gt_heap_validate_out_ptr(void **GT_MAYBE_UNUSED out_ptr)
{
    GT_ASSERT(out_ptr != NULL);
}

static GT_FORCE_INLINE void
__gt_heap_validate_size(size_t GT_MAYBE_UNUSED size)
{
    GT_ASSERT(size > 0);
}

gt_status_t
gt_heap_alloc(void **out_ptr, size_t size)
{
    void *local_ptr;

    __gt_heap_validate_out_ptr(out_ptr);
    __gt_heap_validate_size(size);

    local_ptr = calloc(1, size);
    if (local_ptr == NULL)
    {
        *out_ptr = NULL;
        return GT_STATUS_OUT_OF_MEMORY;
    }

    *out_ptr = local_ptr;

    return GT_STATUS_SUCCESS;
}

void
gt_heap_free(void *ptr)
{
    free(ptr);
}
