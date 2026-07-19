#pragma once

/**
 * @file page.h
 * @brief Platform page size and alignment utilities.
 */

#include <stdbool.h>
#include <stddef.h>

#include <gt/error.h>

gt_status_t
gt_page_init(void);

size_t
gt_page_size(void);

bool
gt_page_is_aligned(const void *address);

bool
gt_page_is_size_aligned(size_t size);

/**
 * Aligns a size upward to the next page boundary.
 */
size_t
gt_page_align_up(size_t size);
