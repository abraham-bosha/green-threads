#pragma once

/**
 * @file log.h
 * @brief Internal runtime logging facilities.
 */

#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>

/* Component Validation */

#ifndef GT_LOG_COMPONENT
#error "GT_LOG_COMPONENT must be defined before including <gt_internal/log/log.h>"
#endif

/* Log Severity Levels */

/**
 * @brief Runtime logging severity levels.
 *
 * Lower values represent more verbose diagnostics.
 * Higher values represent more critical events.
 */
typedef enum
{
    GT_LOG_LEVEL_TRACE = 0,
    GT_LOG_LEVEL_DEBUG,
    GT_LOG_LEVEL_INFO,
    GT_LOG_LEVEL_WARN,
    GT_LOG_LEVEL_ERROR
} gt_log_level_t;

/* Logging Configuration */

#ifndef GT_LOG_LEVEL
#define GT_LOG_LEVEL GT_LOG_LEVEL_DEBUG
#endif

/* Core Logging Implementation */

/**
 * @brief Internal logging implementation.
 *
 * Runtime code should use GT_LOG_* macros instead.
 */
GT_PRINTF_FORMAT(6, 7)
void gt_log(gt_log_level_t level,
            const char *component,
            const char *file,
            int line,
            const char *function,
            const char *format,
            ...);

/* Logging Dispatch */

#ifdef GT_LOG_ENABLED

#define GT_LOG(level, ...)                                                      \
    GT_MACRO_BEGIN                                                              \
    gt_log(level, GT_LOG_COMPONENT, __FILE__, __LINE__, __func__, __VA_ARGS__); \
    GT_MACRO_END

#else

#define GT_LOG(level, ...) GT_UNUSED(0)

#endif

/* Compile-Time Severity Filtering */

#if defined(GT_LOG_ENABLED) && GT_LOG_LEVEL <= GT_LOG_LEVEL_TRACE

#define GT_LOG_TRACE(...) GT_LOG(GT_LOG_LEVEL_TRACE, __VA_ARGS__)

#else

#define GT_LOG_TRACE(...) GT_UNUSED(0)

#endif

#if defined(GT_LOG_ENABLED) && GT_LOG_LEVEL <= GT_LOG_LEVEL_DEBUG

#define GT_LOG_DEBUG(...) GT_LOG(GT_LOG_LEVEL_DEBUG, __VA_ARGS__)

#else

#define GT_LOG_DEBUG(...) GT_UNUSED(0)

#endif

#if defined(GT_LOG_ENABLED) && GT_LOG_LEVEL <= GT_LOG_LEVEL_INFO

#define GT_LOG_INFO(...) GT_LOG(GT_LOG_LEVEL_INFO, __VA_ARGS__)

#else

#define GT_LOG_INFO(...) GT_UNUSED(0)

#endif

#if defined(GT_LOG_ENABLED) && GT_LOG_LEVEL <= GT_LOG_LEVEL_WARN

#define GT_LOG_WARN(...) GT_LOG(GT_LOG_LEVEL_WARN, __VA_ARGS__)

#else

#define GT_LOG_WARN(...) GT_UNUSED(0)

#endif

#if defined(GT_LOG_ENABLED) && GT_LOG_LEVEL <= GT_LOG_LEVEL_ERROR

#define GT_LOG_ERROR(...) GT_LOG(GT_LOG_LEVEL_ERROR, __VA_ARGS__)

#else

#define GT_LOG_ERROR(...) GT_UNUSED(0)

#endif
