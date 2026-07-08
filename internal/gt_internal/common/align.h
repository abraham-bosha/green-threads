#pragma once

#include <assert.h>

#include <gt_internal/common/bits.h>
#include <gt_internal/common/mem.h>

/**
 * @brief Mathematical alignment macros for power-of-two and arbitrary numeric boundaries.
 */

/**
 * @brief Rounds a value up to the nearest multiple of a Power-of-Two boundary.
 */
#define GT_ALIGN_UP_POT(val, align)   ((((val) + ((align) - 1))) & ~((align) - 1))

/**
 * @brief Rounds a value down to the nearest multiple of a Power-of-Two boundary.
 */
#define GT_ALIGN_DOWN_POT(val, align) ((val) & ~((align) - 1))

/**
 * @brief Rounds a value up to the nearest multiple of any arbitrary boundary size.
 */
#define GT_ROUND_UP_ANY(val, align)   ((((val) + (align) - 1) / (align)) * (align))

/**
 * @brief Rounds a value down to the nearest multiple of any arbitrary boundary size.
 */
#define GT_ROUND_DOWN_ANY(val, align) (((val) / (align)) * (align))

/**
 * @brief Universal predicate evaluating if a value perfectly divides by an alignment layer.
 */
#define GT_IS_ALIGNED(val, align)     (((val) % (align)) == 0)

/* Enforce at compile time that pages are powers of two */
static_assert(GT_IS_POW2(GT_PAGE_SIZE), "Error: GT_PAGE_SIZE must be a strict power of two.");
