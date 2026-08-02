# Runtime Bug: Task ID Type Mismatch

## Overview

During the initial implementation of the runtime, task identifiers were defined
as:

```c
typedef uint32_t gt_task_id_t;
```

However, task IDs were allocated through the ID pool subsystem:

```c
gt_id_pool_allocate(&runtime.rt_task_ids, &id);
```

whose interface expected:

```c
gt_status_t
gt_id_pool_allocate(struct gt_id_pool *pool, size_t *id);
```

This introduced a type mismatch between the runtime and the ID pool.

## Root Cause

The runtime declared

```c
gt_task_id_t id;
```

which occupied **4 bytes** on a 64-bit system.

The ID pool, however, interpreted the supplied pointer as

```c
size_t *
```

and therefore wrote an **8-byte** value through that pointer.

Conceptually, the operation looked like:

```text
Runtime

uint32_t id;
      │
      ▼
gt_id_pool_allocate(..., &id)

ID Pool

expects size_t *
```

This violates the function's API contract.

The allocator writes a `size_t`, while the runtime only provides storage for a
`uint32_t`.

This is undefined behavior and may corrupt adjacent memory.

## Resolution

The runtime was updated to define task identifiers as:

```c
typedef size_t gt_task_id_t;
```

This makes the runtime type exactly match the interface expected by
`gt_id_pool_allocate()`.

The allocation path now becomes:

```text
gt_task_id_t
        │
        ▼
size_t
```

with no incompatible pointer conversion and no object size mismatch.

## Lesson Learned

Public subsystem interfaces should agree on the exact object type they operate
on.

Passing a pointer to one type where another type is expected—even if both
represent the same logical value—is undefined behavior when their sizes differ.

Matching the runtime's `gt_task_id_t` to the ID pool's native allocation type
eliminated the mismatch and restored type-safe communication between the two
subsystems.
