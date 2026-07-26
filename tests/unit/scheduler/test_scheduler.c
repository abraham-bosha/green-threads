#include <gt_internal/scheduler/scheduler.h>

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <gt/error.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/task/task.h>

static void
test_scheduler_initial_invariants(void)
{
    struct gt_scheduler sched;
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_scheduler_init(&sched);
    assert(status == GT_STATUS_SUCCESS);

    assert(gt_scheduler_next(&sched) == NULL);
    assert(gt_scheduler_next(&sched) == NULL);
    assert(gt_scheduler_next(&sched) == NULL);

    gt_scheduler_destroy(&sched);
}

static void
test_scheduler_fifo_admission_routing(void)
{
    struct gt_scheduler sched;
    gt_status_t GT_MAYBE_UNUSED status;

    status = gt_scheduler_init(&sched);
    assert(status == GT_STATUS_SUCCESS);

    struct gt_task task1;
    struct gt_task task2;
    struct gt_task task3;

    task1.t_id = 101;
    task1.t_state = GT_TASK_STATE_READY;
    
    task2.t_id = 102;
    task2.t_state = GT_TASK_STATE_READY;
    
    task3.t_id = 103;
    task3.t_state = GT_TASK_STATE_READY;

    status = gt_scheduler_admit(&sched, &task1);
    assert(status == GT_STATUS_SUCCESS);

    status = gt_scheduler_admit(&sched, &task2);
    assert(status == GT_STATUS_SUCCESS);
    
    status = gt_scheduler_admit(&sched, &task3);
    assert(status == GT_STATUS_SUCCESS);

    const struct gt_task *GT_MAYBE_UNUSED out1 = gt_scheduler_next(&sched);
    assert(out1 != NULL);
    assert(out1->t_id == 101);

    const struct gt_task *GT_MAYBE_UNUSED out2 = gt_scheduler_next(&sched);
    assert(out2 != NULL);
    assert(out2->t_id == 102);

    const struct gt_task *GT_MAYBE_UNUSED out3 = gt_scheduler_next(&sched);
    assert(out3 != NULL);
    assert(out3->t_id == 103);

    const struct gt_task *GT_MAYBE_UNUSED final_check = gt_scheduler_next(&sched);
    assert(final_check == NULL);

    gt_scheduler_destroy(&sched);
}

int
main(void)
{
    puts("[RUN] scheduler");

    test_scheduler_initial_invariants();
    test_scheduler_fifo_admission_routing();

    puts("[PASS] scheduler");
    return 0;
}
