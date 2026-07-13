# Module: `list`

## Purpose

The `list` module provides a generic intrusive circular doubly linked list for use throughout the Green Threads runtime.

The module maintains relationships between embedded list nodes while leaving object ownership and lifetime management entirely to the caller.

The list never allocates or frees memory and has no knowledge of the objects containing its nodes.

---

## Responsibilities

The module shall:

* provide intrusive doubly linked list primitives;
* maintain node relationships;
* support constant-time insertion and removal;
* support constant-time movement of nodes between lists;
* provide forward iteration;
* provide safe iteration during node removal;
* detect empty lists and linked nodes.

---

## Non-Responsibilities

The module shall not:

* allocate or free memory;
* manage object lifetime;
* know about runtime objects such as tasks or schedulers;
* perform synchronization;
* perform logging, assertions, tracing, or panic handling;
* implement subsystem-specific policies.

---

## Design Model

The list uses an **intrusive** design.

User-defined objects embed a `struct gt_list_node` directly within their structure.

Example:

```c
struct gt_task
{
    struct gt_list_node ready_node;
};
```

The list operates exclusively on embedded nodes and never accesses the containing object directly.

---

## Circular Sentinel Design

Each list contains a permanent sentinel node.

An empty list is represented by the sentinel pointing to itself in both directions.

```text
      _______
      |     ↓
      |__+------+
         | head |__
         +------+  | 
             ↑_____|
```

A populated list forms a circular chain.

```text
head <-> node1 <-> node2 <-> node3 <-> head
```

Using a circular sentinel removes boundary cases for insertion and removal, allowing every operation to follow the same algorithm.

---

## Ownership Model

The caller owns every object stored in a list.

The list owns only the relationships between embedded nodes.

Removing a node from a list never destroys, allocates, or otherwise affects the lifetime of the containing object.

---

## API

### Types

* `struct gt_list`
* `struct gt_list_node`

### Initialization

* `gt_list_init()`
* `gt_list_node_init()`

### State Queries

* `gt_list_is_empty()`
* `gt_list_node_is_linked()`

### Accessors

* `gt_list_front()`
* `gt_list_back()`

### Insertion

* `gt_list_push_front()`
* `gt_list_push_back()`
* `gt_list_insert_before()`
* `gt_list_insert_after()`

### Removal

* `gt_list_remove()`

### Movement

* `gt_list_move_front()`
* `gt_list_move_back()`

### Iteration

* `GT_LIST_FOR_EACH()`
* `GT_LIST_FOR_EACH_SAFE()`
* `GT_LIST_FOR_EACH_ENTRY()`

---

## Complexity Guarantees

| Operation           | Complexity |
| ------------------- | ---------- |
| List initialization | O(1)       |
| Node initialization | O(1)       |
| Empty check         | O(1)       |
| Linked check        | O(1)       |
| Front access        | O(1)       |
| Back access         | O(1)       |
| Insert              | O(1)       |
| Remove              | O(1)       |
| Move                | O(1)       |
| Forward traversal   | O(n)       |

---

## Invariants

The following conditions must always hold.

### Circular Link Invariant

For every linked node:

```text
node->next->prev == node
node->prev->next == node
```

### Sentinel Invariant

An initialized empty list always satisfies:

```text
head.next == &head
head.prev == &head
```

The sentinel node always remains part of the circular chain and is never removed.

### Node State Invariant

A linked node has valid predecessor and successor pointers.

An unlinked node has both pointers set to `NULL`.

### Ownership Invariant

The list never assumes ownership of the containing object.

### Allocation Invariant

All operations execute without dynamic memory allocation.

---

## Dependency Rules

The list module may depend only on:

* Standard C library headers
* Common infrastructure utilities

The list module must never depend on:

* runtime
* scheduler
* task
* memory
* logging
* assertions
* tracing
* panic

---

## Performance Guarantees

The module guarantees:

* constant-time insertion;
* constant-time removal;
* constant-time movement;
* zero dynamic memory allocation;
* zero global state;
* zero runtime initialization;
* cache-friendly intrusive storage;
* predictable memory layout.

---

## Future Extensions

Future versions may add:

* reverse iteration helpers;
* typed entry iteration helpers;
* list splicing;
* debug consistency validation.

Any future extension shall preserve the intrusive design, circular sentinel representation, and ownership model defined by this contract.

---

**End of Document.**
