#pragma once

#include <gt/error.h>

#include <gt_internal/platform/context/context.h>
#include <gt_internal/platform/heap/heap.h>
#include <gt_internal/platform/page/page.h>
#include <gt_internal/platform/vm/vm.h>

gt_status_t
gt_platform_init(void);

void
gt_platform_destroy(void);
