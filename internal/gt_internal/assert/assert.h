#pragma once

#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/panic/panic.h>

#include <stddef.h>

/**
 * @file assert.h
 * @brief Internal runtime assertion facilities.
 */

#define GT_ASSERT_PREFIX "Assertion Failed: "

/**
 * @brief Reports a failed runtime assertion and transfers control to the panic
 *        subsystem. This function never returns.
 *
 * @param expression The failed assertion expression.
 * @param file       Source file where the assertion failed.
 * @param line       Source line where the assertion failed.
 * @param function   Function containing the assertion.
 * @param message    Optional user-supplied diagnostic message.
 */
GT_NORETURN void
gt_assertion_failed(
    const char *expression, const char *file, int line, const char *function, const char *message);

/**
 * @brief Verifies that a runtime invariant holds.
 *
 * If the condition evaluates to false, execution is terminated through the
 * panic subsystem.
 */
#define GT_ASSERT(cond)                                                               \
    GT_MACRO_BEGIN                                                                    \
    if (GT_UNLIKELY(!(cond)))                                                         \
    {                                                                                 \
        gt_assertion_failed(                                                          \
            GT_ASSERT_PREFIX GT_STRINGIFY(cond), __FILE__, __LINE__, __func__, NULL); \
    }                                                                                 \
    GT_MACRO_END

/**
 * @brief Verifies a runtime invariant with an additional diagnostic message.
 *
 * If the condition evaluates to false, execution is terminated through the
 * panic subsystem after reporting the supplied message.
 */
#define GT_ASSERT_MSG(cond, msg)                                                     \
    GT_MACRO_BEGIN                                                                   \
    if (GT_UNLIKELY(!(cond)))                                                        \
    {                                                                                \
        gt_assertion_failed(                                                         \
            GT_ASSERT_PREFIX GT_STRINGIFY(cond), __FILE__, __LINE__, __func__, msg); \
    }                                                                                \
    GT_MACRO_END
