#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/bitmap/bitmap.h>

/*
 * Test Fixture Context Allocation
 */
struct bitmap_test_context
{
    struct gt_bitmap bitmap;

    /* An array of 2 words to back the bitmap. */
    unsigned long storage[2];
};

static void
setup_context(struct bitmap_test_context *ctx)
{
    /* Initialize with an unaligned bit capacity boundary */
    gt_bitmap_init(&ctx->bitmap, ctx->storage, 75);
}

static void
test_bitmap_init(void)
{
    struct bitmap_test_context ctx;
    setup_context(&ctx);

    /* Verify layout calculations match cached structural metrics */
    assert(ctx.bitmap.total_bits == 75);
    assert(ctx.bitmap.word_count == 2);

    /* Verify core boolean query states */
    assert(gt_bitmap_is_empty(&ctx.bitmap) == true);
    assert(gt_bitmap_is_full(&ctx.bitmap) == false);
}

static void
test_bitmap_mutation_operations(void)
{
    struct bitmap_test_context ctx;
    setup_context(&ctx);

    /* Test standard bit modifications inside Word 0 */
    assert(gt_bitmap_test(&ctx.bitmap, 5) == false);

    gt_bitmap_set(&ctx.bitmap, 5);

    assert(gt_bitmap_test(&ctx.bitmap, 5) == true);
    assert(gt_bitmap_is_empty(&ctx.bitmap) == false);

    gt_bitmap_clear(&ctx.bitmap, 5);
    assert(gt_bitmap_test(&ctx.bitmap, 5) == false);
    assert(gt_bitmap_is_empty(&ctx.bitmap) == true);

    /* Test cross-boundary modifications into the final partial Word 1 */
    assert(gt_bitmap_test(&ctx.bitmap, 70) == false);
    gt_bitmap_set(&ctx.bitmap, 70);
    assert(gt_bitmap_test(&ctx.bitmap, 70) == true);

    assert(ctx.storage[0] == 0UL);
    assert(ctx.storage[1] != 0UL);

    /* Verify memory isolation: Word 0 must remain pure 0, Word 1 must hold the bit */
    gt_bitmap_clear(&ctx.bitmap, 70);
    assert(gt_bitmap_test(&ctx.bitmap, 70) == false);
    assert(gt_bitmap_is_empty(&ctx.bitmap) == true);
}

static void
test_bitmap_bulk_operations(void)
{
    struct bitmap_test_context ctx;
    setup_context(&ctx);

    /* Settle all tracked active bits up to index 74 to 1 */
    gt_bitmap_set_all(&ctx.bitmap);

    assert(gt_bitmap_is_full(&ctx.bitmap) == true);

    /* Shifting Word 1 right by 11 indices must return a clean 0UL. */
    unsigned long GT_MAYBE_UNUSED trailing_padding_check = ctx.storage[1] >> 11;
    assert(trailing_padding_check == 0UL);

    /* Clear back down to baseline and verify */
    gt_bitmap_clear_all(&ctx.bitmap);
    assert(gt_bitmap_is_empty(&ctx.bitmap) == true);
}

static void
test_bitmap_scans(void)
{
    struct bitmap_test_context ctx;
    setup_context(&ctx);

    size_t discovered_bit_index = 999;
    bool GT_MAYBE_UNUSED found;

    /* On an empty map, the first clear bit index must point straight to 0 */
    found = gt_bitmap_find_first_clear(&ctx.bitmap, &discovered_bit_index);
    assert(found == true);
    assert(discovered_bit_index == 0);

    /* Allocate the first block fragment sequentially */
    gt_bitmap_set(&ctx.bitmap, 0);
    gt_bitmap_set(&ctx.bitmap, 1);
    gt_bitmap_set(&ctx.bitmap, 2);

    /* Validate find_first_clear logic */
    found = gt_bitmap_find_first_clear(&ctx.bitmap, &discovered_bit_index);
    assert(found == true);
    assert(discovered_bit_index == 3);

    /* Validate find_first_set logic */
    found = gt_bitmap_find_first_set(&ctx.bitmap, &discovered_bit_index);
    assert(found == true);
    assert(discovered_bit_index == 0);

    /* Clear index 0; first set must shift to index 1 */
    gt_bitmap_clear(&ctx.bitmap, 0);
    found = gt_bitmap_find_first_set(&ctx.bitmap, &discovered_bit_index);
    assert(found == true);
    assert(discovered_bit_index == 1);

    /* Complete saturation check: if full, search must safely return false */
    gt_bitmap_set_all(&ctx.bitmap);
    found = gt_bitmap_find_first_clear(&ctx.bitmap, &discovered_bit_index);
    assert(found == false);
}

int
main(void)
{
    puts("[RUN] ds/bitmap");

    test_bitmap_init();
    test_bitmap_mutation_operations();
    test_bitmap_bulk_operations();
    test_bitmap_scans();

    puts("[PASS] ds/bitmap");
    return 0;
}
