#include <gt/runtime.h>

#include <gt_internal/runtime/runtime.h>

gt_status_t
gt_init(void)
{
    return gt_runtime_init();
}

gt_status_t
gt_create_task(gt_task_entry_fn entry, void *arg, size_t stack_size)
{
    return gt_runtime_create_task(entry, arg, stack_size);
}

void
gt_run(void)
{
    gt_runtime_run();
}

void
gt_yield(void)
{
    gt_runtime_yield();
}

void
gt_destroy(void)
{
    gt_runtime_destroy();
}
