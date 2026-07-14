#include <gt_internal/assert/assert.h>

#include <stdbool.h>
#include <stdio.h>

#include <subprocess.h>

static inline void
test_assert_succeeds(void)
{
    GT_ASSERT(1 == 1);
}

static inline void
test_assert_msg_succeeds(void)
{
    GT_ASSERT_MSG(1 == 1, "should never have be printed");
}

static void
test_assert_fails(void)
{
    GT_ASSERT(false);
}

static void
test_assert_msg_fails(void)
{
    GT_ASSERT_MSG(false, "intentional assert failure");
}

int
main(void)
{
    puts("[RUN] assert");

    test_assert_succeeds();
    test_assert_msg_succeeds();

    verify_process_aborts(test_assert_fails);
    verify_process_aborts(test_assert_msg_fails);

    puts("[PASS] assert");

    return 0;
}
