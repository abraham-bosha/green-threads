#pragma once

/**
 * @brief Mathematical alignment macros for power-of-two and arbitrary numeric boundaries.
 */

/**
 * @brief Rounds a value up to the nearest multiple of a Power-of-Two boundary.
 */
#define GT_ALIGN_UP_POT(val, align) ((((val) + ((align) - 1))) & ~((align) - 1))

/**
 * @brief Rounds a value down to the nearest multiple of a Power-of-Two boundary.
 */
#define GT_ALIGN_DOWN_POT(val, align) ((val) & ~((align) - 1))

/**
 * @brief Rounds a value up to the nearest multiple of any arbitrary boundary size.
 */
#define GT_ALIGN_UP_ANY(val, align) ((((val) + (align) - 1) / (align)) * (align))

/**
 * @brief Rounds a value down to the nearest multiple of any arbitrary boundary size.
 */
#define GT_ALIGN_DOWN_ANY(val, align) (((val) / (align)) * (align))

/**
 * @brief Universal predicate evaluating if a value perfectly divides by an alignment layer.
 */
#define GT_IS_ALIGNED(val, align) (((val) % (align)) == 0)
