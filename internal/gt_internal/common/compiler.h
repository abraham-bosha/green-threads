#pragma once

/**
 * @brief Compiler-specific attributes and optimization abstractions.
 */

#if defined(__clang__) || defined(__GNUC__)

/**
 * @brief Marks a condition as likely to evaluate to true.
 */
#define GT_LIKELY(x) __builtin_expect(!!(x), 1)

/**
 * @brief Marks a condition as unlikely to evaluate to true.
 */
#define GT_UNLIKELY(x) __builtin_expect(!!(x), 0)

/**
 * @brief Prevents the compiler from inlining a function.
 */
#define GT_NOINLINE __attribute__((noinline))

/**
 * @brief Forces the compiler to inline a function whenever possible.
 */
#define GT_FORCE_INLINE __attribute__((always_inline)) inline

/**
 * @brief Marks a function as never returning.
 */
#define GT_NORETURN __attribute__((noreturn))

/**
 * @brief Warns if the caller ignores the function's return value.
 */
#define GT_WARN_UNUSED_RESULT __attribute__((warn_unused_result))

/**
 * @brief Applies printf-style format checking to variadic functions.
 *
 * @param fmt_index 1-based index of the format string parameter.
 * @param arg_index 1-based index of the first variadic argument.
 */
#define GT_PRINTF_FORMAT(fmt_index, arg_index) \
    __attribute__((format(printf, fmt_index, arg_index)))

/**
 * @brief Aligns an object to the specified byte boundary.
 */
#define GT_ALIGNED(n) __attribute__((aligned(n)))

/**
 * @brief Removes compiler-inserted structure padding.
 */
#define GT_PACKED __attribute__((packed))

/**
 * @brief Indicates an intentional switch fallthrough.
 */
#define GT_FALLTHROUGH __attribute__((fallthrough))

/**
 * @brief Marks a code path as unreachable.
 */
#define GT_UNREACHABLE() __builtin_unreachable()

#else

#define GT_LIKELY(x) (x)
#define GT_UNLIKELY(x) (x)

#define GT_NOINLINE
#define GT_FORCE_INLINE inline
#define GT_NORETURN

#define GT_WARN_UNUSED_RESULT
#define GT_PRINTF_FORMAT(fmt_index, arg_index)

#define GT_ALIGNED(n)
#define GT_PACKED

#define GT_FALLTHROUGH ((void)0)
#define GT_UNREACHABLE() ((void)0)

#endif
