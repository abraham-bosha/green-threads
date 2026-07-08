#pragma once

#include <gt_internal/common/compiler.h>

/**
 * @file panic.h
 * @brief Internal fatal error reporting service.
 */

#define GT_PANIC_BANNER "GREEN THREADS PANIC"

/**
 * @brief Reports an unrecoverable runtime failure and terminates the process.
 *
 * This function never returns. It prints diagnostic information describing the
 * failure before aborting the process.
 *
 * @param file     Source file where the panic originated.
 * @param line     Source line where the panic originated.
 * @param function Function in which the panic occurred.
 * @param message  The description of the violated invariant.
 * @note Returns nothing.
 */
GT_NORETURN void
gt_panic(
    const char *filename,
    int line,
    const char *function,
    const char *message);

/**
 * @brief Reports a fatal runtime failure using the current source location.
 *
 * Automatically captures the source file, function name, and line number,
 * reducing boilerplate and ensuring consistent diagnostics.
 */
#define GT_PANIC(message) gt_panic(__FILE__, __LINE__, __func__, (message)) 
