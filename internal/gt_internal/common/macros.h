#pragma once

/**
 * @brief Begins a safe multi-statement macro expansion.
 */
#define GT_MACRO_BEGIN \
    do                 \
    {
/**
 * @brief Ends a safe multi-statement macro expansion.
 */
#define GT_MACRO_END \
    }                \
    while (0)

/**
 * @brief Two-step preprocessor macros to safely convert code tokens into string literals.
 */
#define GT_STRINGIFY_HELPER(x) #x
#define GT_STRINGIFY(x) GT_STRINGIFY_HELPER(x)

/**
 * @brief Two-step preprocessor macros to safely merge two code tokens into a new single token.
 */
#define GT_CONCAT_HELPER(a, b) a##b
#define GT_CONCAT(a, b) GT_CONCAT_HELPER(a, b)

/**
 * @brief Computes the byte offset of a member item relative to its parent base boundary.
 */
#define GT_OFFSET_OF(type, member) offsetof(type, member)

/**
 * @brief Casts a pointer to a nested structural element back into a pointer to its parent enclosure
 * container.
 */
#define GT_CONTAINER_OF(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
