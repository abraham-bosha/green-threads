#include <gt/version.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <gt_internal/common/compiler.h>

/*
 * Version subsystem contract test.
 *
 * Validates consistency between:
 *   - compile-time version macros
 *   - runtime version functions
 *
 * Ensures no mismatch between headers and linked library.
 */

static void
test_compile_time(void)
{
    assert(GT_VERSION_MAJOR == 1);
    assert(GT_VERSION_MINOR == 0);
    assert(GT_VERSION_PATCH == 0);

    assert(GT_VERSION_NUMBER == 10000);
    assert(strcmp(GT_VERSION_STRING, "1.0.0") == 0);
}

static void
test_runtime(void)
{
    assert(strcmp(gt_version_string(), "1.0.0") == 0);

    uint32_t GT_MAYBE_UNUSED v = gt_version_number();
    assert(v == 10000);
    assert(v == GT_VERSION_NUMBER);
}

int
main(void)
{
    printf("[RUN] runtime/version\n");

    test_compile_time();
    test_runtime();

    printf("[PASS] runtime/version\n");

    return 0;
}
