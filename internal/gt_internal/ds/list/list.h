#pragma once

/**
 * @file list.h
 * @brief Intrusive circular doubly linked list.
 *
 * This module implements a generic intrusive doubly linked list using a
 * circular sentinel node. The list never allocates or frees memory; it only
 * maintains relationships between user-owned nodes.
 *
 * Every list contains a permanent sentinel head node. An empty list is
 * represented by the head pointing to itself in both directions.
 */

#include <stdbool.h>
#include <stddef.h>

#include <gt_internal/common/compiler.h>
#include <gt_internal/common/macros.h>

/*
 * List Invariant
 *
 * For every linked node:
 *
 *      node->next->prev == node
 *      node->prev->next == node
 *
 * Empty list:
 *
 *      head.next == &head
 *      head.prev == &head
 */

/**
 * @brief Intrusive list node link tracking.
 *
 * Embed this structure inside user-defined objects to make them linkable.
 * A node may belong to at most one list at any given time.
 */
struct gt_list_node
{
    struct gt_list_node *prev;
    struct gt_list_node *next;
};

/**
 * @brief Circular list master anchor.
 *
 * The head is a permanent sentinel node and is never removed from the list.
 */
struct gt_list
{
    struct gt_list_node head;
};

/**
 * @brief Splices a new node cleanly between two known adjacent nodes.
 *
 * @pre prev and next are adjacent nodes.
 * @pre node is not currently linked into another list.
 */
static GT_FORCE_INLINE void
__gt_list_link(struct gt_list_node *node, struct gt_list_node *prev, struct gt_list_node *next)
{
    next->prev = node;
    node->next = next;
    prev->next = node;
    node->prev = prev;
}

/**
 * @brief Removes a node from its current list.
 *
 * Internal primitive used by all removal operations.
 *
 * After unlinking, the node is marked as detached by clearing its links.
 *
 * @pre node is currently linked into a list.
 */
static GT_FORCE_INLINE void
__gt_list_unlink(struct gt_list_node *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    node->prev = NULL;
    node->next = NULL;
}

/**
 * @brief Initializes an empty list achor.
 */
static GT_FORCE_INLINE void
gt_list_init(struct gt_list *list)
{
    list->head.prev = &list->head;
    list->head.next = &list->head;
}

/**
 * @brief Initializes a standalone list node.
 *
 * A newly initialized node is not linked into any list.
 */
static GT_FORCE_INLINE void
gt_list_node_init(struct gt_list_node *node)
{
    node->prev = NULL;
    node->next = NULL;
}

/**
 * @brief Returns whether the list contains no active user nodes.
 */
static GT_FORCE_INLINE bool
gt_list_is_empty(const struct gt_list *list)
{
    return (list->head.prev == &list->head && list->head.next == &list->head);
}

/**
 * @brief Returns whether a node currently resides inside a list.
 */
static GT_FORCE_INLINE bool
gt_list_node_is_linked(const struct gt_list_node *node)
{
    return (node->prev != NULL && node->next != NULL);
}

/**
 * @brief Returns a borrowed pointer to the first real data node in the list.
 * @return The front data node, or NULL if the collection is empty.
 */
static GT_FORCE_INLINE struct gt_list_node *
gt_list_front(const struct gt_list *list)
{
    return gt_list_is_empty(list) ? NULL : list->head.next;
}

/**
 * @brief Returns a borrowed pointer to the last real data node in the list.
 * @return The back data node, or NULL if the collection is empty.
 */
static GT_FORCE_INLINE struct gt_list_node *
gt_list_back(const struct gt_list *list)
{
    return gt_list_is_empty(list) ? NULL : list->head.prev;
}

/**
 * @brief Inserts an isolated node immediately before another target node.
 */
static GT_FORCE_INLINE void
gt_list_insert_before(struct gt_list_node *pos, struct gt_list_node *node)
{
    __gt_list_link(node, pos->prev, pos);
}

/**
 * @brief Inserts an isolated node immediately after another target node.
 */
static GT_FORCE_INLINE void
gt_list_insert_after(struct gt_list_node *pos, struct gt_list_node *node)
{
    __gt_list_link(node, pos, pos->next);
}

/**
 * @brief Inserts an isolated node at the absolute front of the list (LIFO).
 */
static GT_FORCE_INLINE void
gt_list_push_front(struct gt_list *list, struct gt_list_node *node)
{
    gt_list_insert_after(&list->head, node);
}

/**
 * @brief Inserts an isolated node at the absolute back of the list (FIFO).
 */
static GT_FORCE_INLINE void
gt_list_push_back(struct gt_list *list, struct gt_list_node *node)
{
    gt_list_insert_before(&list->head, node);
}

/**
 * @brief Removes an active data node from its current list.
 */
static GT_FORCE_INLINE void
gt_list_remove(struct gt_list_node *node)
{
    __gt_list_unlink(node);
}

/**
 * @brief Moves an active linked node to the absolute front of the target list.
 */
static GT_FORCE_INLINE void
gt_list_move_front(struct gt_list *list, struct gt_list_node *node)
{
    __gt_list_unlink(node);
    gt_list_push_front(list, node);
}

/**
 * @brief Moves an active linked node to the absolute back of the target list.
 */
static GT_FORCE_INLINE void
gt_list_move_back(struct gt_list *list, struct gt_list_node *node)
{
    __gt_list_unlink(node);
    gt_list_push_back(list, node);
}

/**
 * @brief Iterates over every node in forward order.
 * @param pos  The moving cursor tracking pointer variable.
 * @param list The pointer to the target master gt_list container.
 */
#define GT_LIST_FOR_EACH(pos, list) \
    for ((pos) = (list)->head.next; (pos) != &(list)->head; (pos) = (pos)->next)

/**
 * @brief Iterates safely forward while allowing the current node to be removed.
 * @param pos  The moving cursor tracking pointer variable.
 * @param nxt  The temporary safety cursor variable to cache the next node step.
 * @param list The pointer to the target master gt_list container.
 */
#define GT_LIST_FOR_EACH_SAFE(pos, nxt, list)                                    \
    for ((pos) = (list)->head.next, (nxt) = (pos)->next; (pos) != &(list)->head; \
         (pos) = (nxt), (nxt) = (pos)->next)

/**
 * @brief Iterates over the containing parent objects rather than raw list nodes.
 * @param pos    The typed pointer cursor representing the parent struct type.
 * @param list   The pointer to the target master gt_list container.
 * @param member The literal variable name of the embedded structural node field.
 */
#define GT_LIST_FOR_EACH_ENTRY(pos, list, member)                                      \
    for (struct gt_list_node *__curr = (list)->head.next; (__curr != &(list)->head) && \
         (((pos) = GT_CONTAINER_OF(__curr, typeof(*(pos)), member)), true);            \
         __curr = __curr->next)
