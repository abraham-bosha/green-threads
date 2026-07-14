#pragma once

/**
 * @file queue.h
 * @brief Intrusive first-in, first-out (FIFO) queue.
 *
 * This module provides a lightweight intrusive FIFO queue built on top of
 * the list module. The queue owns no memory and performs no pointer
 * manipulation directly; all structural operations are delegated to the
 * underlying list implementation.
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/ds/list/list.h>

/**
 * @brief Intrusive FIFO queue.
 *
 * The queue stores embedded list nodes while preserving insertion order.
 */
struct gt_queue
{
    struct gt_list head;
};

/**
 * @brief Initializes an empty queue.
 */
static GT_FORCE_INLINE void
gt_queue_init(struct gt_queue *q)
{
    assert(q != NULL);

    gt_list_init(&q->head);
}

/**
 * @brief Returns whether the queue contains no elements.
 */
static GT_FORCE_INLINE bool
gt_queue_is_empty(const struct gt_queue *q)
{
    assert(q != NULL);

    return gt_list_is_empty(&q->head);
}

/**
 * @brief Returns the element at the front of the queue.
 *
 * Returns NULL if the queue is empty.
 *
 * This operation does not remove the element.
 */
static GT_FORCE_INLINE struct gt_list_node *
gt_queue_peek(const struct gt_queue *q)
{
    assert(q != NULL);

    return gt_list_front(&q->head);
}

/**
 * @brief Inserts an element at the back of the queue.
 *
 * @pre The node is not currently linked into another container.
 */
static GT_FORCE_INLINE void
gt_queue_enqueue(struct gt_queue *q, struct gt_list_node *node)
{
    assert(q != NULL);

    gt_list_push_back(&q->head, node);
}

/**
 * @brief Removes and returns the front element.
 *
 * Returns NULL if the queue is empty.
 */
static GT_FORCE_INLINE struct gt_list_node *
gt_queue_dequeue(struct gt_queue *q)
{
    assert(q != NULL);

    struct gt_list_node *front_node = gt_list_front(&q->head);

    if (front_node != NULL)
    {
        gt_list_remove(front_node);
    }

    return front_node;
}
