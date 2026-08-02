#pragma once

#include <gt_internal/common/types.h>

extern int
__gt_context_save_asm(struct gt_context *current);

extern GT_NORETURN void
__gt_context_load_asm(const struct gt_context *next);
