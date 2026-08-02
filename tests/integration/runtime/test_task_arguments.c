#include <gt_internal/runtime/runtime.h>
#include <gt_internal/runtime/runtime_internal.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/list/list.h>

struct test_payload_struct
{
    uint32_t magic_field;
    uint64_t data_field;
    char status_flag;
};

static volatile bool g_integer_argument_verified = false;
static volatile bool g_structure_argument_verified = false;
static volatile bool g_null_argument_verified = false;

static void
task_integer_argument_entry(void *arg)
{
    assert(arg != NULL);

    const int *value = arg;

    assert(*value == 8888);

    g_integer_argument_verified = true;
}

static void
task_structure_argument_entry(void *arg)
{
    assert(arg != NULL);

    const struct test_payload_struct *payload = arg;

    assert(payload->magic_field == 0xCAFEBABE);
    assert(payload->data_field == 0xDEADBEEFDEFEC8EDULL);
    assert(payload->status_flag == 'G');

    g_structure_argument_verified = true;
}

static void
task_null_argument_entry(void *arg)
{
    const void *GT_MAYBE_UNUSED null_ptr = arg;

    assert(null_ptr == NULL);

    g_null_argument_verified = true;
}

/*
 * Verifies that task arguments are delivered to entry functions exactly
 * as supplied during task creation.
 *
 * Integer pointers, structure pointers, and NULL arguments should all
 * be preserved without modification.
 */
static void
test_task_argument_integrity(void)
{
    gt_status_t status = gt_runtime_init();
    assert(status == GT_STATUS_SUCCESS);

    const size_t stack_size = 16 * 1024;

    int integer_payload = 8888;

    struct test_payload_struct structure_payload = {
        .magic_field = 0xCAFEBABE,
        .data_field = 0xDEADBEEFDEFEC8EDULL,
        .status_flag = 'G',
    };

    status = gt_runtime_create_task(task_integer_argument_entry, &integer_payload, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_structure_argument_entry, &structure_payload, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_runtime_create_task(task_null_argument_entry, NULL, stack_size);
    assert(status == GT_STATUS_SUCCESS);

    assert(runtime.rt_task_count == 3);

    gt_runtime_run();

    assert(g_integer_argument_verified);
    assert(g_structure_argument_verified);
    assert(g_null_argument_verified);

    assert(runtime.rt_state == GT_RUNTIME_STATE_STOPPED);
    assert(runtime.rt_current == NULL);

    assert(runtime.rt_task_count == 0);

    assert(gt_list_is_empty(&runtime.rt_tasks));
    assert(gt_list_is_empty(&runtime.rt_zombies));

    gt_runtime_destroy();
}

int
main(void)
{
    puts("[RUN] integration/runtime_task_arguments");

    test_task_argument_integrity();

    puts("[PASS] integration/runtime_task_arguments");
    return 0;
}
