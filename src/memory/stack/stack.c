#include <gt_internal/memory/stack/stack.h>

#include <stddef.h>
#include <stdint.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/align.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/mem.h>
#include <gt_internal/platform/page/page.h>
#include <gt_internal/platform/vm/vm.h>

static GT_FORCE_INLINE void
__gt_stack_validate_stack(const struct gt_stack *GT_MAYBE_UNUSED s)
{
    GT_ASSERT(s != NULL);
}

static GT_FORCE_INLINE void
__gt_stack_validate_base(void *base)
{
    GT_ASSERT(GT_IS_ALIGNED((uintptr_t)base, GT_STACK_ALIGNMENT));
}

static GT_FORCE_INLINE void
__gt_stack_validate_size(size_t size)
{
    GT_ASSERT(GT_IS_ALIGNED(size, GT_STACK_ALIGNMENT));
}

static GT_FORCE_INLINE void
__gt_stack_clear(struct gt_stack *GT_MAYBE_UNUSED s)
{
    gt_mem_clear(s, sizeof(*s));
}

gt_status_t
gt_stack_init(struct gt_stack *s, size_t stack_size)
{
    __gt_stack_validate_stack(s);
    __gt_stack_clear(s);

    struct gt_vm_mapping usable_mapping;

    size_t page_size;
    size_t mapping_size;
    size_t guard_size;

    gt_status_t status;

    if (stack_size == 0UL)
    {
        stack_size = GT_STACK_DEFAULT_SIZE;
    } else
    {
        stack_size = gt_page_align_up(stack_size);
    }

    page_size = gt_page_size();
    guard_size = GT_STACK_GUARD_PAGES * page_size;
    mapping_size = stack_size + guard_size;

    status = gt_vm_mapping_reserve(&s->mapping, mapping_size);
    if (status != GT_STATUS_SUCCESS)
    {
        return status;
    }

    __gt_stack_validate_base(s->mapping.vm_base);
    __gt_stack_validate_size(s->mapping.vm_size);

    /*
     * Exclude the leading guard pages to obtain the usable execution stack.
     */
    usable_mapping = (struct gt_vm_mapping){
        .vm_base = (void *)((uintptr_t)s->mapping.vm_base + page_size),
        .vm_size = s->mapping.vm_size - guard_size,
    };

    /*
     * Enable read/write access only for the usable stack region.
     */
    status = gt_vm_mapping_protect(&usable_mapping, GT_VM_ACCESS_READ_WRITE);
    if (status != GT_STATUS_SUCCESS)
    {
        /*
         * Roll back the reserved mapping if stack initialization fails.
         */
        gt_vm_mapping_release(&s->mapping);
        __gt_stack_clear(s);

        return status;
    }

    __gt_stack_validate_base(usable_mapping.vm_base);
    __gt_stack_validate_size(usable_mapping.vm_size);

    s->stack_base = usable_mapping.vm_base;
    s->stack_size = usable_mapping.vm_size;

    return GT_STATUS_SUCCESS;
}

void
gt_stack_destroy(struct gt_stack *s)
{
    __gt_stack_validate_stack(s);

    gt_vm_mapping_release(&s->mapping);

    __gt_stack_clear(s);
}
