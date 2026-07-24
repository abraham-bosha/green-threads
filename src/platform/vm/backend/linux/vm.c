/**
 * @file vm.c
 * @brief Linux implementation of the platform virtual memory interface.
 */
#include <errno.h>
#include <stdint.h>
#include <sys/mman.h>

#include <gt/error.h>

#include <gt_internal/assert/assert.h>
#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>
#include <gt_internal/platform/page/page.h>
#include <gt_internal/platform/vm/vm.h>

/* -------------------------------------------------------------------------- */
/* Internal Validation Helpers                                                */
/* -------------------------------------------------------------------------- */

static GT_FORCE_INLINE void
__gt_vm_mapping_validate_mapping(const struct gt_vm_mapping *GT_MAYBE_UNUSED m)
{
    GT_ASSERT(m != NULL);
}

static GT_FORCE_INLINE void
__gt_vm_mapping_validate_alignment(const void *GT_MAYBE_UNUSED vm_base)
{
    GT_ASSERT(vm_base != NULL);
    GT_ASSERT(gt_page_is_aligned(vm_base));
}

static GT_FORCE_INLINE void
__gt_vm_mapping_validate_size(size_t GT_MAYBE_UNUSED vm_size)
{
    GT_ASSERT(vm_size > 0UL);
    GT_ASSERT(gt_page_is_size_aligned(vm_size));
}

static GT_FORCE_INLINE void
__gt_vm_mapping_validate_access(gt_vm_access_t GT_MAYBE_UNUSED access)
{
    GT_ASSERT_MSG(false, "Unsupported virtual memory access mode.");
}

/* -------------------------------------------------------------------------- */
/* Internal Helper                                                            */
/* -------------------------------------------------------------------------- */

/**
 * @brief Resets a virtual memory mapping descriptor to an empty state.
 */
static GT_FORCE_INLINE void
__gt_vm_mapping_reset(struct gt_vm_mapping *m)
{
    __gt_vm_mapping_validate_mapping(m);

    m->vm_base = NULL;
    m->vm_size = 0UL;
}

/* -------------------------------------------------------------------------- */
/* Platform Translation Helper                                                */
/* -------------------------------------------------------------------------- */

static inline int
__gt_vm_mapping_translate_access(gt_vm_access_t access)
{
    switch (access)
    {
        case GT_VM_ACCESS_NONE:
            return PROT_NONE;

        case GT_VM_ACCESS_READ_WRITE:
            return PROT_READ | PROT_WRITE;

        default:
            __gt_vm_mapping_validate_access(access);
    }

    return PROT_NONE;
}

/* -------------------------------------------------------------------------- */
/* Public API Implementations                                                 */
/* -------------------------------------------------------------------------- */

gt_status_t
gt_vm_mapping_reserve(struct gt_vm_mapping *m, size_t vm_size)
{
    __gt_vm_mapping_validate_mapping(m);
    __gt_vm_mapping_validate_size(vm_size);

    void *base;

    /* Reserve anonymous virtual base space with no access permissions. */
    base = mmap(NULL, vm_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (base == MAP_FAILED)
    {
        __gt_vm_mapping_reset(m);

        switch (errno)
        {
            case ENOMEM:
                return GT_STATUS_OUT_OF_MEMORY;

            default:
                return GT_STATUS_VM_RESERVATION_FAILED;
        }
    }

    m->vm_base = base;
    m->vm_size = vm_size;

    return GT_STATUS_SUCCESS;
}

gt_status_t
gt_vm_mapping_protect(const struct gt_vm_mapping *m, gt_vm_access_t access)
{
    __gt_vm_mapping_validate_mapping(m);
    __gt_vm_mapping_validate_alignment(m->vm_base);
    __gt_vm_mapping_validate_size(m->vm_size);

    int prot = __gt_vm_mapping_translate_access(access);

    /* Apply the requested page protection. */
    int status = mprotect(m->vm_base, m->vm_size, prot);

    if (status != 0)
    {
        return GT_STATUS_VM_PROTECTION_FAILED;
    }

    return GT_STATUS_SUCCESS;
}

gt_status_t
gt_vm_mapping_release(struct gt_vm_mapping *m)
{
    __gt_vm_mapping_validate_mapping(m);
    __gt_vm_mapping_validate_alignment(m->vm_base);
    __gt_vm_mapping_validate_size(m->vm_size);

    /* Release the virtual memory mapping. */
    int status = munmap(m->vm_base, m->vm_size);

    if (status != 0)
    {
        return GT_STATUS_VM_RELEASE_FAILED;
    }

    /* Reset the descriptor after releasing the mapping. */
    __gt_vm_mapping_reset(m);

    return GT_STATUS_SUCCESS;
}
