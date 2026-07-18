#pragma once

#include <limits.h>

/**
 * @brief Foundational bitwise widths and primitive hardware mask generators.
 */

/**
 * @brief The physical number of bits contained within a single standard byte.
 */
#define GT_BITS_PER_BYTE CHAR_BIT

/**
 * @brief The number of bits contained within a native storage word.
 *
 * Defines the internal block width used by bitmap storage.
 */
#define GT_BITS_PER_WORD (sizeof(unsigned long) * GT_BITS_PER_BYTE)

/**
 * @brief Compile-time predicate evaluating whether a non-zero integer is a perfect power of two.
 */
#define GT_IS_POW2(val) (((val) & ((val) - 1)) == 0)
