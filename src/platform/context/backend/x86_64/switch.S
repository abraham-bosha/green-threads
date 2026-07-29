#include "context_offsets.h"

.text

/*
 * Saves the current execution context.
 *
 * System V AMD64 calling convention:
 *
 *      rdi -> current context
 *
 * Returns:
 *      0 immediately after saving the context.
 *      1 when the saved context is later resumed.
 */

.global __gt_context_save_asm
.type __gt_context_save_asm, @function
__gt_context_save_asm:

    /* Save callee-saved registers. */
    movq %rsp, GT_CONTEXT_RSP_OFFSET(%rdi)
    movq %rbx, GT_CONTEXT_RBX_OFFSET(%rdi)
    movq %rbp, GT_CONTEXT_RBP_OFFSET(%rdi)
    movq %r12, GT_CONTEXT_R12_OFFSET(%rdi)
    movq %r13, GT_CONTEXT_R13_OFFSET(%rdi)
    movq %r14, GT_CONTEXT_R14_OFFSET(%rdi)
    movq %r15, GT_CONTEXT_R15_OFFSET(%rdi)

    /*
     * First return from gt_context_save().
     */
    xorl %eax, %eax

    ret

.size __gt_context_save_asm, .-__gt_context_save_asm


/*
 * Restores a previously saved execution context.
 *
 * System V AMD64 calling convention:
 *
 *      rdi -> context to restore
 *
 * This routine never returns normally.
 */

.global __gt_context_load_asm
.type __gt_context_load_asm, @function
__gt_context_load_asm:

    /* Restore callee-saved registers. */
    movq GT_CONTEXT_RSP_OFFSET(%rdi), %rsp
    movq GT_CONTEXT_RBX_OFFSET(%rdi), %rbx
    movq GT_CONTEXT_RBP_OFFSET(%rdi), %rbp
    movq GT_CONTEXT_R12_OFFSET(%rdi), %r12
    movq GT_CONTEXT_R13_OFFSET(%rdi), %r13
    movq GT_CONTEXT_R14_OFFSET(%rdi), %r14
    movq GT_CONTEXT_R15_OFFSET(%rdi), %r15

    /*
     * When execution resumes, gt_context_save()
     * returns 1.
     */
    movl $1, %eax

    ret

.size __gt_context_load_asm, .-__gt_context_load_asm


/*
 * Bootstrap routine for a newly configured execution context.
 *
 * Initial stack layout:
 *
 *      rsp -> gt_context_start
 *              entry function
 *              entry argument
 *
 * After the first context restore:
 *
 *      entry(entry_argument)
 *
 * This routine must never return.
 */

.global gt_context_start
.type gt_context_start, @function
gt_context_start:

    /*
     * Pop the configured entry function and its
     * argument from the prepared stack frame.
     */
    popq %rax
    popq %rdi

    call *%rax

    /*
     * A task entry function must never return.
     * The runtime trampoline is responsible for
     * terminating the task.
     */
    ud2

.size gt_context_start, .-gt_context_start


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",@progbits
#endif
