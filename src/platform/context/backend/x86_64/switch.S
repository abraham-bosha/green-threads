#include "context_offsets.h"

.text

/*
 * Saves the current execution context and resumes another.
 *
 * Calling convention (System V AMD64):
 *   rdi -> current context
 *   rsi -> next context
 */
.global gt_context_switch
.type gt_context_switch, @function
gt_context_switch:

    # Save the current execution context.
    movq %rsp, GT_CONTEXT_RSP_OFFSET(%rdi)
    movq %rbx, GT_CONTEXT_RBX_OFFSET(%rdi)
    movq %rbp, GT_CONTEXT_RBP_OFFSET(%rdi)
    movq %r12, GT_CONTEXT_R12_OFFSET(%rdi)
    movq %r13, GT_CONTEXT_R13_OFFSET(%rdi)
    movq %r14, GT_CONTEXT_R14_OFFSET(%rdi)
    movq %r15, GT_CONTEXT_R15_OFFSET(%rdi)

    # Restore the next execution context.
    movq GT_CONTEXT_RSP_OFFSET(%rsi), %rsp
    movq GT_CONTEXT_RBX_OFFSET(%rsi), %rbx
    movq GT_CONTEXT_RBP_OFFSET(%rsi), %rbp
    movq GT_CONTEXT_R12_OFFSET(%rsi), %r12
    movq GT_CONTEXT_R13_OFFSET(%rsi), %r13
    movq GT_CONTEXT_R14_OFFSET(%rsi), %r14
    movq GT_CONTEXT_R15_OFFSET(%rsi), %r15

    # Resume execution using the return address on the restored stack.
    ret

.size gt_context_switch, .-gt_context_switch

/*
 * Bootstrap for a newly initialized execution context.
 *
 * Initial stack layout:
 *
 *      rsp -> entry function
 *              entry argument
 *
 * Invokes:
 *
 *      entry(entry_argument)
 *
 * Never returns.
 */
.global gt_context_start
.type gt_context_start, @function
gt_context_start:

    popq %rax        # entry function
    popq %rdi        # first argument (System V ABI)

    call *%rax

    # A context entry function must never return.
    ud2

.size gt_context_start, .-gt_context_start


#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",@progbits
#endif
