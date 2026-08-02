# Heap Module

## Purpose

The Heap module provides a minimal abstraction over dynamic heap memory allocation.

It isolates the runtime from direct use of the C standard library allocation routines, allowing allocation behavior to be replaced or extended without affecting higher-level subsystems.

Typical future uses include:

- Custom allocators
- Memory pools
- Debug allocators
- Allocation statistics
- Leak detection
- Platform-specific allocation strategies

---

## Responsibilities

The Heap module is responsible for:

- Allocating heap memory
- Releasing heap memory
- Translating allocation failures into runtime status codes

The module does **not** perform:

- Memory pooling
- Garbage collection
- Stack allocation
- Virtual memory management
- Guard page management

Those belong to separate subsystems.

---

## Public Interface

```c
gt_status_t
gt_heap_alloc(void **ptr, size_t size);

void
gt_heap_free(void *ptr);
```

---

## Ownership

Memory returned by `gt_heap_alloc()` is owned exclusively by the caller.

The caller is responsible for eventually releasing it using `gt_heap_free()`.

Every successful allocation must have exactly one corresponding free.

---

## Error Handling

`gt_heap_alloc()` returns `GT_STATUS_OUT_OF_MEMORY` when allocation fails.

`gt_heap_free()` accepts `NULL`; freeing a null pointer has no effect.

---

## Dependencies

The Heap module depends only on:

- Platform C runtime (`calloc`, `free`)
- Common infrastructure
- Error definitions

It has no dependency on the runtime, scheduler, task, or context subsystems.

---

## Design Notes

The Heap module intentionally exposes a very small API.

Keeping the interface minimal makes it straightforward to replace the underlying allocator in the future without impacting the rest of the runtime.

---

End of Document.
