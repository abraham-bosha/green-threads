#include <subprocess.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define GT_LOG_COMPONENT "test"

#include <gt_internal/log/log.h>
#include <gt_internal/common/compiler.h>

static void
emit_log(void)
{
    GT_LOG_INFO("runtime initialized");
}

static void
verify_log(const char *output)
{
    assert(strstr(output, "INFO"));
    assert(strstr(output, "test"));
    assert(strstr(output, "runtime initialized"));

    GT_UNUSED(output);
}

int
main(void)
{
    puts("[RUN] log");

    verify_process_output(emit_log, verify_log);

    puts("[PASS] log");

    return 0;
}
