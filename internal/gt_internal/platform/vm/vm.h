#pragma once

#include <stddef.h>

#include <gt/error.h>

typedef enum
{
    GT_VM_ACCESS_NONE = 0,
    GT_VM_ACCESS_READ_WRITE = 1,
} gt_vm_access_t;

struct gt_vm_mapping
{
    void *vm_base;
    size_t vm_size;
};

gt_status_t
gt_vm_mapping_reserve(struct gt_vm_mapping *m, size_t vm_size);

gt_status_t
gt_vm_mapping_protect(const struct gt_vm_mapping *m, gt_vm_access_t acess);

gt_status_t
gt_vm_mapping_release(struct gt_vm_mapping *m);
