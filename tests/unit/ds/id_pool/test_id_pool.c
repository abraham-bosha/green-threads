#include <gt_internal/ds/id_pool/id_pool.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include <gt_internal/common/compiler.h>

struct id_pool_test_context
{
    struct gt_id_pool pool;
    unsigned long storage; /* A single word handles up to 64 unique identifiers. */
};

static void
setup_test_context(struct id_pool_test_context *ctx)
{
    /* Initialize an ID pool with 32 identifiers */
    gt_id_pool_init(&ctx->pool, &ctx->storage, 32);
}

static void
test_id_pool_init(void)
{
    struct id_pool_test_context ctx;
    setup_test_context(&ctx);

    assert(gt_id_pool_is_empty(&ctx.pool) == true);
    assert(gt_id_pool_is_full(&ctx.pool) == false);
}

static void
test_id_pool_allocation(void)
{
    struct id_pool_test_context ctx;
    setup_test_context(&ctx);

    size_t id1 = 999;
    size_t id2 = 999;

    /* Allocate first ID; must be strictly 0-indexed */
    bool GT_MAYBE_UNUSED alloc1 = gt_id_pool_allocate(&ctx.pool, &id1);
    assert(alloc1 == true);
    assert(id1 == 0);
    assert(gt_id_pool_is_allocated(&ctx.pool, id1) == true);

    /* Allocate second ID; must increment sequentially */
    bool GT_MAYBE_UNUSED alloc2 = gt_id_pool_allocate(&ctx.pool, &id2);
    assert(alloc2 == true);
    assert(id2 == 1);
    assert(gt_id_pool_is_allocated(&ctx.pool, id2) == true);

    /* Release ID 0 and verify state mutation */
    gt_id_pool_release(&ctx.pool, id1);
    assert(gt_id_pool_is_allocated(&ctx.pool, id1) == false);

    /* Recycle and verify state allocation safety */
    size_t recycled_id = 999;
    bool GT_MAYBE_UNUSED alloc3 = gt_id_pool_allocate(&ctx.pool, &recycled_id);
    assert(alloc3 == true);
    assert(recycled_id == 0); /* Reused the released slot */
}

static void
test_id_pool_saturation_limits(void)
{
    struct id_pool_test_context ctx;
    gt_id_pool_init(&ctx.pool, &ctx.storage, 4); /* Small pool for fast saturation */

    size_t GT_MAYBE_UNUSED temp_id;

    /* Saturate all 4 available allocation tracks */
    assert(gt_id_pool_allocate(&ctx.pool, &temp_id) == true);  // ID 0
    assert(gt_id_pool_allocate(&ctx.pool, &temp_id) == true);  // ID 1
    assert(gt_id_pool_allocate(&ctx.pool, &temp_id) == true);  // ID 2
    assert(gt_id_pool_allocate(&ctx.pool, &temp_id) == true);  // ID 3

    assert(gt_id_pool_is_full(&ctx.pool) == true);

    /* Subsequent allocations must fail cleanly */
    bool GT_MAYBE_UNUSED failed_alloc = gt_id_pool_allocate(&ctx.pool, &temp_id);
    assert(failed_alloc == false);
}

int
main(void)
{
    puts("[RUN] ds/id_pool");

    test_id_pool_init();
    test_id_pool_allocation();
    test_id_pool_saturation_limits();

    puts("[PASS] ds/id_pool");
    return 0;
}
