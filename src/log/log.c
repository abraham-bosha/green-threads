#include <stdarg.h>
#include <stdio.h>

#define GT_LOG_COMPONENT "log"

#include <gt_internal/log/log.h>

static const char *
gt_log_level_string(gt_log_level_t level)
{
    switch (level)
    {
        case GT_LOG_LEVEL_TRACE:
            return "TRACE";

        case GT_LOG_LEVEL_DEBUG:
            return "DEBUG";

        case GT_LOG_LEVEL_INFO:
            return "INFO ";

        case GT_LOG_LEVEL_WARN:
            return "WARN ";

        case GT_LOG_LEVEL_ERROR:
            return "ERROR";

        default:
            return "<invalid>";
    }
}

void
gt_log(gt_log_level_t level,
       const char *component,
       const char *file,
       int line,
       const char *function,
       const char *format,
       ...)
{
    va_list args;

    FILE *stream = stderr;

    fprintf(stream,
            "[%s][%s] %s:%d %s(): ",
            gt_log_level_string(level),
            component,
            file,
            line,
            function);

    va_start(args, format);

    vfprintf(stream, format, args);

    va_end(args);
}
