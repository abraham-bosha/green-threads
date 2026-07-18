# ID Pool Module

## Purpose

The ID Pool module manages the allocation and recycling of unique integer identifiers.

It provides a simple and efficient mechanism for acquiring and releasing IDs while ensuring that no active ID is allocated more than once.

---

## Responsibilities

The ID Pool module is responsible for:

- Allocating unique IDs.
- Releasing IDs back to the pool.
- Recycling released IDs.
- Tracking ID availability.
- Detecting pool exhaustion.

---

## Non-Responsibilities

The ID Pool module does **not**:

- Store objects associated with IDs.
- Manage object lifetimes.
- Allocate or free user objects.
- Provide synchronization.
- Persist IDs across program execution.

---

## Dependencies

The module may depend on:

- Common
- Bitmap

The module must **not** depend on:

- Runtime
- Scheduler
- Task
- Context
- Memory

---

## Ownership

The caller owns the ID pool instance.

The module owns all internal data structures required to manage IDs.

Released IDs return to the pool and may be allocated again.

---

## Guarantees

The module guarantees:

- Each allocated ID is unique while active.
- Released IDs become available for future allocation.
- An ID is never allocated twice simultaneously.
- Allocation fails when no IDs remain.

---

## Performance

|   Operation     |   Complexity    |
|-----------------|-----------------|
| Create          | O(n)            |
| Destroy         | O(1)            |
| Allocate ID     | O(n) worst case |
| Release ID      | O(1)            |
| Test Allocation | O(1)            |

---

## Failure Conditions

Operations may fail if:

- The pool cannot be created.
- The pool is exhausted.
- An invalid ID is released.
- Invalid arguments are supplied.

---

## Invariants

The following conditions must always hold:

- Every active ID is unique.
- Every free ID is available for allocation.
- No ID is simultaneously active and free.
- The number of active IDs never exceeds the pool capacity.
- Released IDs may be reused.
- Internal bookkeeping remains consistent after every operation.

---

## Future Extensions

Future versions may support:

- Constant-time allocation.
- Dynamic pool growth.
- Thread-safe allocation.
- Lock-free implementations.
- Generation counters to detect stale IDs.

---

End of Document
