# Bitmap Module

## Purpose

The bitmap module provides an efficient representation of a fixed number of binary states.

Each bit represents one independent boolean value. A bit is either **set (1)** or **clear (0)**.

The bitmap operates on caller-provided storage and never allocates or owns memory.

---

# Design

The bitmap is a **non-owning** data structure.

The caller provides:

* the bitmap object;
* the backing storage;
* the total number of valid bits.

The bitmap is responsible only for manipulating and querying bits within the supplied storage.

Memory allocation, ownership, synchronization, and lifetime management remain the caller's responsibility.

---

# Representation

Internally, bits are stored in an array of machine words.

The implementation uses `unsigned long` as the storage unit to naturally match the underlying architecture.

The storage layout is an implementation detail and is never exposed through the public API.

Users interact exclusively through bit indices.

---

# Responsibilities

The bitmap module is responsible for:

* initializing a bitmap over caller-provided storage;
* setting individual bits;
* clearing individual bits;
* toggling individual bits;
* testing individual bits;
* setting all valid bits;
* clearing all valid bits;
* finding the first set bit;
* finding the first clear bit.

---

# Non-Responsibilities

The bitmap module does **not**:

* allocate or free memory;
* resize storage;
* store arbitrary objects;
* implement resource allocation;
* manage ownership;
* perform synchronization.

---

# Invariants

The following invariants shall always hold.

## Valid Bit Range

If a bitmap contains `total_bits`, then the only valid bit indices are:

```text
0 ... total_bits - 1
```

Accessing any other bit index violates the bitmap contract.

---

## Trailing Bits

The backing storage may contain unused bits in the final machine word.

All trailing bits outside `total_bits` shall always remain cleared (`0`).

This guarantees that bitmap search operations never return invalid bit indices.

---

## Ownership

The bitmap never assumes ownership of its backing storage.

The caller is responsible for allocating, maintaining, and releasing the storage.

---

## Representation

Each valid bit represents exactly one boolean state.

No operation may modify any unrelated bit.

---

## Encapsulation

The public API exposes only bit indices.

Word calculations, bit masks, compiler intrinsics, and storage layout remain internal implementation details.

---

# Complexity

| Operation        |                    Complexity |
| ---------------- | ----------------------------- |
| Initialization   |                          O(1) |
| Test Bit         |                          O(1) |
| Set Bit          |                          O(1) |
| Clear Bit        |                          O(1) |
| Toggle Bit       |                          O(1) |
| Clear All        |            O(number of words) |
| Set All          |            O(number of words) |
| Find First Set   | O(number of words) worst case |
| Find First Clear | O(number of words) worst case |

---

# Dependencies

The bitmap module depends only on the Common subsystem.

It has no dependency on:

* List
* Queue
* Scheduler
* Memory
* Runtime

---

# Future Users

The bitmap is intended to serve as foundational infrastructure for higher-level runtime components, including:

* ID pools;
* scheduler state;
* stack pool management;
* worker management;
* reactor bookkeeping;
* runtime resource tracking.

---

# Design Principles

The bitmap follows the project's core engineering principles.

* Operates on caller-owned memory.
* Exposes abstraction, not representation.
* Keeps implementation details private.
* Uses `static GT_FORCE_INLINE` helper functions instead of function-like macros.
* Optimizes internally using compiler intrinsics where appropriate without exposing them through the public interface.
* Maintains deterministic behavior by guaranteeing that all trailing bits remain permanently cleared.

---

End of Document
