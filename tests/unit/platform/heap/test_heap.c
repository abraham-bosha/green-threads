#include <gt_internal/platform/heap/heap.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/compiler.h>

/**
 * @brief Validates a standard allocation lifecycle and zero-initialization invariants.
 */
static void
test_heap_standard_lifecycle_and_zeroing(void)
{
    void *ptr = NULL;
    size_t alloc_size = 128UL;

    gt_status_t status = gt_heap_alloc(&ptr, alloc_size);
    assert(status == GT_STATUS_SUCCESS);
    assert(ptr != NULL);

    unsigned char *byte_ptr = (unsigned char *)ptr;
    for (size_t i = 0UL; i < alloc_size; i++)
    {
        assert(byte_ptr[i] == 0x00);
    }

    byte_ptr[0] = 0xAA;
    byte_ptr[alloc_size - 1UL] = 0xBB;
    assert(byte_ptr[0] == 0xAA);
    assert(byte_ptr[alloc_size - 1UL] == 0xBB);

    gt_heap_free(ptr);
}

/**
 * @brief Validates out-of-memory boundary traps and NULL output pointer states.
 */
static void
test_heap_out_of_memory_boundaries(void)
{
    void *ptr = (void *)0xDEADBEEFUL;

    size_t catastrophic_size = SIZE_MAX;

    gt_status_t status = gt_heap_alloc(&ptr, catastrophic_size);
    assert(status == GT_STATUS_OUT_OF_MEMORY);
    assert(ptr == NULL);
}

int
main(void)
{
    puts("[RUN] platform/heap");

    test_heap_standard_lifecycle_and_zeroing();
    test_heap_out_of_memory_boundaries();

    puts("[PASS] platform/heap");
    return 0;
}
