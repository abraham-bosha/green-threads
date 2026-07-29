#pragma once

/**
 * @file heap.h
 * @brief Platform heap allocation abstraction.
 */

#include <stddef.h>

#include <gt/error.h>

/**
 * @brief Allocates heap memory.
 *
 * Allocates a contiguous memory region of the requested size.
 * Ownership of the allocated memory is transferred to the caller.
 */
gt_status_t
gt_heap_alloc(void **out_ptr, size_t size);

/**
 * @brief Releases previously allocated heap memory.
 *
 * Passing NULL has no effect.
 */
void
gt_heap_free(void *ptr);
