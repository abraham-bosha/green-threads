#include <gt_internal/task/task.h>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/platform/page/page.h>

static void
mock_user_workload(void *arg)
{
    int *payload = (int *)arg;
    (*payload)++;
}

static void
test_task_initialization_invariants(void)
{
    struct gt_task task;
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_page_init();
    assert(status == GT_STATUS_SUCCESS);

    gt_task_id_t expected_id = 42;
    int mock_payload = 100;
    size_t custom_stack_sz = gt_page_size() * 2UL; /* 8KB Stack */

    status = gt_task_init(&task, expected_id, mock_user_workload, &mock_payload, custom_stack_sz);
    assert(status == GT_STATUS_SUCCESS);

    assert(task.t_id == expected_id);
    assert(task.t_state == GT_TASK_STATE_NEW);
    assert(task.t_entry == mock_user_workload);
    assert(task.t_arg == &mock_payload);

    assert(task.t_stack.s_base != NULL);
    assert(task.t_stack.s_size >= custom_stack_sz);

    assert(task.t_context.rsp != 0);

    uintptr_t GT_MAYBE_UNUSED stack_limit_top =
        (uintptr_t)task.t_stack.s_base + task.t_stack.s_size;
    assert((uintptr_t)task.t_context.rsp == stack_limit_top);
    assert((uintptr_t)task.t_context.rsp >= (uintptr_t)task.t_stack.s_base);

    gt_task_destroy(&task);

    assert(task.t_id == 0);
    assert(task.t_stack.s_base == NULL);
    assert(task.t_stack.s_size == 0UL);
}

int
main(void)
{
    puts("[RUN] task");

    test_task_initialization_invariants();

    puts("[PASS] task");
    return 0;
}
