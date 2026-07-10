#pragma once

#include <limits.h>
#include <stdint.h>

/**
 * @brief Foundational bitwise widths and primitive hardware mask generators.
 */

/**
 * @brief The physical number of bits contained within a single standard byte.
 */
#define GT_BITS_PER_BYTE CHAR_BIT

/**
 * @brief The physical number of bits contained within a single 64-bit storage word.
 *        Directly establishes the internal block element width for the ds/bitmap system.
 */
#define GT_BITS_PER_WORD (sizeof(uint64_t) * GT_BITS_PER_BYTE)

/**
 * @brief Generates an isolated, architecture-agnostic 64-bit unsigned bitmask
 *        containing a single active '1' at the requested position.
 * @param pos The bit target offset index position (MUST sit strictly between 0 and 63).
 */
#define GT_BIT64(pos) (UINT64_C(1) << (pos))

/**
 * @brief Compile-time predicate evaluating whether a non-zero integer is a perfect power of two.
 */
#define GT_IS_POW2(val) (((val) & ((val) - 1)) == 0)
