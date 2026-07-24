#pragma once

/**
 * @file bitmap.h
 * @brief Non-owning bitmap implementation.
 */

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/bits.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/mem.h>

/**
 * @brief Caller-owned bitmap.
 *
 * The bitmap never allocates or owns its backing storage.
 */
struct gt_bitmap
{
    unsigned long *words;
    size_t total_bits;
    size_t total_words;
};

/* -------------------------------------------------------------------------- */
/* Internal Helpers                                                           */
/* -------------------------------------------------------------------------- */

static GT_FORCE_INLINE size_t
__gt_bitmap_word_index(size_t bit_index)
{
    return bit_index / GT_BITS_PER_WORD;
}

static GT_FORCE_INLINE unsigned int
__gt_bitmap_bit_offset(size_t bit_index)
{
    return bit_index % GT_BITS_PER_WORD;
}

static GT_FORCE_INLINE unsigned long
__gt_bitmap_bit_mask(unsigned int bit_offset)
{
    return 1UL << bit_offset;
}

static GT_FORCE_INLINE size_t
__gt_bitmap_total_words(size_t total_bits)
{
    return (total_bits + GT_BITS_PER_WORD - 1UL) / GT_BITS_PER_WORD;
}

static GT_FORCE_INLINE unsigned long
__gt_bitmap_last_word_mask(size_t total_bits)
{
    unsigned int bit_offset = (size_t)__gt_bitmap_bit_offset(total_bits);
    return (bit_offset != 0) ? (__gt_bitmap_bit_mask(bit_offset) - 1UL) : ~0UL;
}

/* -------------------------------------------------------------------------- */
/* Validation Helpers                                                         */
/* -------------------------------------------------------------------------- */

static GT_FORCE_INLINE void
__gt_bitmap_validate_bitmap(const struct gt_bitmap *GT_MAYBE_UNUSED b)
{
    GT_ASSERT(b != NULL);
}

static GT_FORCE_INLINE void
__gt_bitmap_validate_storage(const unsigned long *GT_MAYBE_UNUSED words)
{
    GT_ASSERT(words != NULL);
}

static GT_FORCE_INLINE void
__gt_bitmap_validate_bit_index(const struct gt_bitmap *GT_MAYBE_UNUSED b,
                               size_t GT_MAYBE_UNUSED bit_index)
{
    GT_ASSERT(bit_index < b->total_bits);
}

static GT_FORCE_INLINE void
__gt_bitmap_validate_total_bits(size_t GT_MAYBE_UNUSED total_bits)
{
    GT_ASSERT(total_bits > 0UL);
}

static GT_FORCE_INLINE void
__gt_bitmap_validate_out_bit_index(const size_t *GT_MAYBE_UNUSED out_bit_index)
{
    GT_ASSERT(out_bit_index != NULL);
}

/* -------------------------------------------------------------------------- */
/* Bitmap Queries                                                             */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initializes a bitmap over caller-provided storage.
 */
static GT_FORCE_INLINE void
gt_bitmap_init(struct gt_bitmap *b, unsigned long *words, size_t total_bits)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(words);
    __gt_bitmap_validate_total_bits(total_bits);

    size_t total_words;
    total_words = __gt_bitmap_total_words(total_bits);

    b->words = words;
    b->total_bits = total_bits;
    b->total_words = total_words;

    gt_mem_clear(words, sizeof(*words) * total_words);
}

static GT_FORCE_INLINE bool
gt_bitmap_is_empty(const struct gt_bitmap *b)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);

    for (size_t i = 0; i < b->total_words; i++)
    {
        if (b->words[i] != 0UL)
        {
            return false;
        }
    }

    return true;
}

static GT_FORCE_INLINE bool
gt_bitmap_is_full(const struct gt_bitmap *b)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);

    size_t full_total_words = b->total_bits / GT_BITS_PER_WORD;

    bool has_partial_word = b->total_words > full_total_words;

    for (size_t i = 0; i < full_total_words; i++)
    {
        if (b->words[i] != ~0UL)
        {
            return false;
        }
    }

    if (has_partial_word)
    {
        unsigned long partial_word_mask = __gt_bitmap_last_word_mask(b->total_bits);
        if (b->words[full_total_words] != partial_word_mask)
        {
            return false;
        }
    }

    return true;
}

static GT_FORCE_INLINE bool
gt_bitmap_test(const struct gt_bitmap *b, size_t bit_index)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);
    __gt_bitmap_validate_bit_index(b, bit_index);

    size_t word_index = __gt_bitmap_word_index(bit_index);
    unsigned int bit_offset = __gt_bitmap_bit_offset(bit_index);
    unsigned long bit_mask = __gt_bitmap_bit_mask(bit_offset);

    return (b->words[word_index] & bit_mask) != 0UL;
}

/* -------------------------------------------------------------------------- */
/* Modification Operations                                                    */
/* -------------------------------------------------------------------------- */

static GT_FORCE_INLINE void
gt_bitmap_set(struct gt_bitmap *b, size_t bit_index)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);
    __gt_bitmap_validate_bit_index(b, bit_index);

    size_t word_index = __gt_bitmap_word_index(bit_index);
    unsigned int bit_offset = __gt_bitmap_bit_offset(bit_index);
    unsigned long bit_mask = __gt_bitmap_bit_mask(bit_offset);

    b->words[word_index] |= bit_mask;
}

static GT_FORCE_INLINE void
gt_bitmap_clear(struct gt_bitmap *b, size_t bit_index)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);
    __gt_bitmap_validate_bit_index(b, bit_index);

    size_t word_index = __gt_bitmap_word_index(bit_index);
    unsigned int bit_offset = __gt_bitmap_bit_offset(bit_index);
    unsigned long bit_mask = __gt_bitmap_bit_mask(bit_offset);

    b->words[word_index] &= ~bit_mask;
}

/* -------------------------------------------------------------------------- */
/* Bulk Operations                                                            */
/* -------------------------------------------------------------------------- */

/**
 * @brief Sets every valid bit.
 *
 * Trailing bits outside total_bits remain cleared.
 */
static GT_FORCE_INLINE void
gt_bitmap_set_all(struct gt_bitmap *b)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);

    gt_mem_set(b->words, sizeof(*b->words) * b->total_words);

    b->words[b->total_words - 1] &= __gt_bitmap_last_word_mask(b->total_bits);
}

static GT_FORCE_INLINE void
gt_bitmap_clear_all(struct gt_bitmap *b)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);

    gt_mem_clear(b->words, sizeof(*b->words) * b->total_words);
}

/* -------------------------------------------------------------------------- */
/* Search Operations                                                          */
/* -------------------------------------------------------------------------- */

/**
 * @brief Finds the first set bit.
 *
 * Returns false if no set bit exists.
 */
static GT_FORCE_INLINE bool
gt_bitmap_find_first_set(const struct gt_bitmap *b, size_t *out_bit_index)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);
    __gt_bitmap_validate_out_bit_index(out_bit_index);

    for (size_t i = 0; i < b->total_words; i++)
    {
        if (b->words[i] != 0UL)
        {
            size_t bit_index = __builtin_ctzl(b->words[i]);
            *out_bit_index = (i * GT_BITS_PER_WORD) + bit_index;
            return true;
        }
    }

    return false;
}

/**
 * @brief Finds the first clear bit.
 *
 * Returns false if no clear bit exists.
 */
static GT_FORCE_INLINE bool
gt_bitmap_find_first_clear(const struct gt_bitmap *b, size_t *out_bit_index)
{
    __gt_bitmap_validate_bitmap(b);
    __gt_bitmap_validate_storage(b->words);
    __gt_bitmap_validate_out_bit_index(out_bit_index);

    unsigned long partial_word_mask;

    partial_word_mask = __gt_bitmap_last_word_mask(b->total_bits);

    for (size_t i = 0; i < b->total_words; i++)
    {
        unsigned long inverted_word = ~b->words[i];

        if (i == b->total_words - 1)
        {
            inverted_word &= partial_word_mask;
        }

        if (inverted_word != 0UL)
        {
            size_t bit_index = (size_t)__builtin_ctzl(inverted_word);
            *out_bit_index = (i * GT_BITS_PER_WORD) + bit_index;
            return true;
        }
    }

    return false;
}
