#pragma once

/**
 * @file id_pool.h
 * @brief Deterministic reusable identifier allocator.
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/bitmap/bitmap.h>

/**
 * @brief Caller-owned ID allocator.
 *
 * Each bit represents one identifier:
 *
 *   bit = 0  -> available
 *   bit = 1  -> allocated
 *
 * The pool does not own its backing storage.
 */
struct gt_id_pool
{
    struct gt_bitmap map;
};

/* -------------------------------------------------------------------------- */
/* Validation Helpers                                                         */
/* -------------------------------------------------------------------------- */

static GT_FORCE_INLINE void
__gt_id_pool_validate(const struct gt_id_pool *GT_MAYBE_UNUSED p)
{
    assert(p != NULL);
}

static GT_FORCE_INLINE void
__gt_id_pool_validate_storage(const unsigned long *GT_MAYBE_UNUSED storage)
{
    assert(storage != NULL);
}

static GT_FORCE_INLINE void
__gt_id_pool_validate_total_ids(size_t GT_MAYBE_UNUSED total_ids)
{
    assert(total_ids > 0UL);
}

static GT_FORCE_INLINE void
__gt_id_pool_validate_id(const struct gt_id_pool *GT_MAYBE_UNUSED p, size_t GT_MAYBE_UNUSED id)
{
    assert(id < p->map.total_bits);
}

static GT_FORCE_INLINE void
__gt_id_pool_validate_out_id(const size_t *GT_MAYBE_UNUSED out_id)
{
    assert(out_id != NULL);
}

/**
 * @brief Ensures an ID is currently allocated.
 *
 * Releasing a free ID is a programming error.
 */
static GT_FORCE_INLINE void
__gt_id_pool_validate_allocated(struct gt_id_pool *GT_MAYBE_UNUSED p, size_t GT_MAYBE_UNUSED id)
{
    assert(gt_bitmap_test(&p->map, id) == true);
}

/* -------------------------------------------------------------------------- */
/* Initialization                                                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initializes an ID pool over caller-provided storage.
 *
 * All identifiers are initially available.
 */
static GT_FORCE_INLINE void
gt_id_pool_init(struct gt_id_pool *p, unsigned long *storage, size_t total_ids)
{
    __gt_id_pool_validate(p);
    __gt_id_pool_validate_storage(storage);
    __gt_id_pool_validate_total_ids(total_ids);

    gt_bitmap_init(&p->map, storage, total_ids);
}

/* -------------------------------------------------------------------------- */
/* Allocation                                                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Allocates the lowest available identifier.
 *
 * @return true if allocation succeeded.
 * @return false if no identifier is available.
 */
static GT_FORCE_INLINE bool
gt_id_pool_allocate(struct gt_id_pool *p, size_t *out_id)
{
    __gt_id_pool_validate(p);
    __gt_id_pool_validate_out_id(out_id);

    if (!gt_bitmap_find_first_clear(&p->map, out_id))
    {
        return false;
    }

    gt_bitmap_set(&p->map, *out_id);
    return true;
}

/**
 * @brief Releases an allocated identifier.
 *
 * The identifier must currently be allocated.
 */
static GT_FORCE_INLINE void
gt_id_pool_release(struct gt_id_pool *p, size_t id)
{
    __gt_id_pool_validate(p);
    __gt_id_pool_validate_id(p, id);
    __gt_id_pool_validate_allocated(p, id);

    gt_bitmap_clear(&p->map, id);
}

/* -------------------------------------------------------------------------- */
/* Queries                                                                    */
/* -------------------------------------------------------------------------- */

/**
 * @brief Checks whether an identifier is allocated.
 */
static GT_FORCE_INLINE bool
gt_id_pool_is_allocated(const struct gt_id_pool *p, size_t id)
{
    __gt_id_pool_validate(p);
    __gt_id_pool_validate_id(p, id);

    return gt_bitmap_test(&p->map, id);
}

/**
 * @brief Checks whether no identifiers are allocated.
 */
static GT_FORCE_INLINE bool
gt_id_pool_is_empty(const struct gt_id_pool *p)
{
    __gt_id_pool_validate(p);

    return gt_bitmap_is_empty(&p->map);
}

/**
 * @brief Checks whether all identifiers are allocated.
 */
static GT_FORCE_INLINE bool
gt_id_pool_is_full(const struct gt_id_pool *p)
{
    __gt_id_pool_validate(p);

    return gt_bitmap_is_full(&p->map);
}
