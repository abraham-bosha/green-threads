# Common Module Contract

The **Common** module provides the low-level infrastructure shared by every subsystem in the Green Threads runtime. It contains lightweight, header-only utilities that establish the project's fundamental language, compiler, memory, alignment, and bit manipulation abstractions.

This module forms the lowest layer of the dependency graph and must remain independent of higher-level runtime components.

---

# Module: `types.h`

## Purpose

Defines the common semantic types shared across the runtime. It serves as the project's central type registry by providing semantic type aliases, runtime enumerations, callback signatures, and opaque object declarations.

## Responsibilities

- Define semantic runtime types.
- Define runtime-wide enumerations.
- Define shared callback signatures.
- Forward declare runtime objects.
- Eliminate circular header dependencies.

## Design Rules

- Do not redefine standard C primitive types.
- Domain-specific types must use explicit names (e.g. `gt_task_id_t`).
- Structure typedef aliases are prohibited.
- Runtime objects must remain opaque.
- Implementation details must never be exposed.

---

# Module: `macros.h`

## Purpose

Provides reusable preprocessor utilities used throughout the runtime.

## Responsibilities

- Define generic utility macros.
- Remove duplicated preprocessor logic.
- Improve compile-time readability.

## Rules

- Header-only.
- Zero runtime overhead.
- Independent of all runtime modules.

---

# Module: `compiler.h`

## Purpose

Provides portable wrappers around compiler-specific attributes and optimization hints.

## Responsibilities

- Abstract compiler extensions.
- Provide optimization hints.
- Provide attribute wrappers.
- Supply portable fallbacks.

## Invariants

- Compiler wrappers must never alter program semantics.
- `GT_NORETURN` functions must never return.
- Every compiler extension must have a safe fallback.

---

# Module: `bits.h`

## Purpose

Provides compile-time bit manipulation primitives and bit-related constants.

## Responsibilities

- Define bit-width constants.
- Generate bit masks.
- Provide reusable bit operations.

## Invariants

- Shift operations must remain within the width of the underlying integer type.
- All operations must evaluate at compile time.

---

# Module: `mem.h`

## Purpose

The mem module provides common memory operations used throughout the runtime.

It offers small, reusable abstractions over standard C memory primitives while keeping the runtime independent of direct C library usage.

---

## Responsibilities

The Memory Utilities module shall:

- Provide common memory operations.
- Expose lightweight runtime memory helpers.
- Hide direct use of C standard library memory functions where appropriate.

---

## Non-Responsibilities

The Memory Utilities module shall not:

- Allocate memory.
- Manage virtual memory.
- Manage runtime stacks.
- Provide platform-specific memory functionality.

---

## Relationships

The Memory Utilities module depends on:

- Common
- C Standard Library

---

# Module: `align.h`

## Purpose

Provides alignment utilities used by allocators, stacks, and low-level address arithmetic.

## Responsibilities

- Align values upward.
- Align values downward.
- Check alignment.
- Encapsulate alignment calculations.

## Invariants

- Bit-mask alignment requires non-zero power-of-two alignment values.
- Alignment operations must always produce mathematically correct results.
- Architectural assumptions should be verified with compile-time assertions whenever possible.

---

# Dependency Rules

The Common module may depend only on:

- Standard C library headers
- Compiler built-ins
- Implementation-defined language facilities

The Common module must never depend on:

- Runtime
- Scheduler
- Task
- Context
- Memory allocator
- Platform
- Data structures
- Logging
- Tracing
- Assertions
- Panic services

---

# Performance Guarantees

- Header-only abstractions where practical.
- Zero dynamic memory allocation.
- Zero global state.
- Zero runtime initialization.
- Compile-time evaluation whenever possible.
- No hidden runtime overhead.

---

End of Document
