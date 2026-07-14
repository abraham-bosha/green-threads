# Module: `queue`

## Purpose

The `queue` module provides a lightweight intrusive First-In, First-Out (FIFO) queue for use throughout the Green Threads runtime.

The queue expresses FIFO semantics while delegating all structural operations to the underlying `list` module.

The queue never allocates or frees memory and never manipulates list pointers directly.

---

## Responsibilities

The queue module shall:

* provide an intrusive FIFO queue abstraction;
* support constant-time enqueue and dequeue operations;
* support constant-time peek of the next element;
* support constant-time empty checks;
* maintain FIFO ordering;
* delegate all structural operations to the `list` module.

---

## Non-Responsibilities

The queue module shall not:

* allocate or free memory;
* manage object lifetime;
* perform synchronization;
* perform pointer manipulation;
* expose arbitrary list operations;
* expose iteration interfaces;
* expose access to the back of the queue.

---

## Design

The queue is implemented as a thin wrapper around the intrusive list.

```text
queue
    ↓
list
```

The queue exists to express FIFO semantics, not to introduce another linked list implementation.

---

## Ownership Model

The queue owns only the relationships between embedded list nodes.

The caller owns every object stored in the queue.

Removing an element from the queue never destroys, allocates, or otherwise affects the lifetime of the containing object.

---

## Public API

### Types

* `struct gt_queue`

### Initialization

* `gt_queue_init()`

### Queries

* `gt_queue_is_empty()`
* `gt_queue_peek()`

### Modification

* `gt_queue_enqueue()`
* `gt_queue_dequeue()`

---

## Complexity Guarantees

| Operation      | Complexity |
| -------------- | ---------: |
| Initialization |       O(1) |
| Empty check    |       O(1) |
| Peek           |       O(1) |
| Enqueue        |       O(1) |
| Dequeue        |       O(1) |

---

## Invariants

The following conditions must always hold.

### FIFO Invariant

Elements leave the queue in the same order they entered it.

```text
enqueue A
enqueue B
enqueue C

↓

dequeue → A
dequeue → B
dequeue → C
```

---

### Empty Queue Invariant

For an empty queue:

* `gt_queue_is_empty()` returns `true`.
* `gt_queue_peek()` returns `NULL`.

---

### Ownership Invariant

The queue never owns the objects it contains.

It only manages relationships between embedded list nodes.

---

### Allocation Invariant

All queue operations execute without dynamic memory allocation.

---

### Layering Invariant

The queue module shall never manipulate list pointers directly.

All structural modifications shall be performed exclusively through the public interfaces of the `list` module.

---

### Abstraction Invariant

The queue exposes only FIFO operations.

Operations that violate the FIFO abstraction, such as arbitrary insertion, arbitrary removal, reverse traversal, or iteration, are intentionally omitted.

---

## Dependencies

The queue module may depend only on:

* `common`
* `list`

The queue module must not depend on any higher-level runtime subsystem.

---

## Dependents

The queue module is intended to be used by:

* scheduler
* runtime
* reactor
* timer
* synchronization primitives
* worker scheduling

---

## Performance Goals

The queue guarantees:

* constant-time operations;
* zero dynamic memory allocation;
* zero pointer manipulation outside the `list` module;
* zero global state;
* predictable FIFO behavior.

---

## Future Extensions

Future versions may add convenience helpers or typed wrappers.

Any future extension shall preserve the FIFO abstraction and the layering relationship between the `queue` and `list` modules.

---

**End of Document**
