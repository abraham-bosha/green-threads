#pragma once

/**
 * @file mem.h
 * @brief Common runtime memory utilities.
 */

#include <stddef.h>
#include <string.h>

#include <gt_internal/common/compiler.h>

/**
 * @brief Fills a memory region with zeros.
 */
static GT_FORCE_INLINE void
gt_mem_clear(void *ptr, size_t size)
{
    memset(ptr, 0, size);
}

/**
 * @brief Fills a memory region with ones.
 */
static GT_FORCE_INLINE void
gt_mem_set(void *ptr, size_t size)
{
    memset(ptr, 0xFF, size);
}

/**
 * @brief Copies bytes from one memory region to another.
 *
 * The source and destination regions shall not overlap.
 */
static GT_FORCE_INLINE void
gt_mem_copy(void *dst, const void *src, size_t size)
{
    memcpy(dst, src, size);
}

/**
 * @brief Moves bytes between memory regions.
 *
 * The source and destination regions may overlap.
 */
static GT_FORCE_INLINE void
gt_mem_move(void *dst, const void *src, size_t size)
{
    memmove(dst, src, size);
}
