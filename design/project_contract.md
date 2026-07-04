# Project Contract

## Project

green\_threads

## Version

Version 1.0

## Status

Frozen

---

# Purpose

green\_threads is a user-space cooperative green-thread runtime written in C.

The project exists to study and implement the fundamental building blocks of modern language runtimes, schedulers, and asynchronous execution systems while maintaining a clean, modular, and production-quality architecture.

Version 1 prioritizes correctness, simplicity, readability, and extensibility over performance.

---

# Objectives

Version 1 must demonstrate a complete cooperative runtime capable of:

- initializing the runtime
- creating tasks
- allocating task stacks
- switching execution contexts
- cooperative yielding
- scheduling runnable tasks
- destroying completed tasks
- shutting down the runtime cleanly

The runtime should be small enough to understand completely while providing a strong foundation for future versions.

---

# Scope

Version 1 includes:

- user-space cooperative multitasking
- a single scheduler
- a single execution thread
- FIFO scheduling
- dynamically allocated task stacks
- ucontext backend
- reusable data structures
- modular runtime architecture
- unit and integration tests
- example programs
- performance benchmarks

---

# Out of Scope

The following are intentionally excluded from Version 1.

## Scheduling

- preemptive scheduling
- priorities
- deadlines
- fairness algorithms
- work stealing
- worker threads
- NUMA awareness

## Context Switching

- setjmp()/longjmp() backend
- assembly backend

## Memory

- stack pools
- guard pages
- custom allocators
- memory arenas
- garbage collection

## Synchronization

- mutexes
- spinlocks
- atomics
- lock-free structures

## Timing

- timers
- sleeping tasks
- timer wheel
- timerfd

## Networking

- sockets
- epoll
- kqueue
- io\_uring
- asynchronous I/O

## Runtime

- cancellation
- task migration
- affinity
- task local storage

---

# Design Principles

The runtime shall follow these principles.

## Simplicity

Choose the simplest correct implementation.

---

## Correctness

Correct behavior always takes priority over performance.

---

## Modularity

Each subsystem owns one responsibility.

---

## Encapsulation

Public interfaces must not expose implementation details.

---

## Explicit Ownership

Every resource has exactly one owner responsible for releasing it.

---

## Predictable Control Flow

Execution flow should remain easy to follow and debug.

---

## Minimal Dependencies

Subsystems depend only on lower architectural layers.

Circular dependencies are forbidden.

---

## Testability

Every subsystem should be independently testable.

---

## Portability

Version 1 targets POSIX systems.

Linux is the primary development platform.

---

# Architectural Invariants

The following rules must remain true throughout Version 1.

- A task exists in exactly one lifecycle state at any time.
- Only the scheduler decides which task executes next.
- Context backends do not implement scheduling policy.
- Public APIs never expose internal runtime structures.
- Ownership of runtime-managed resources is always explicit.
- Dependencies flow downward through architectural layers.

---

# Architecture

Version 1 consists of the following architectural modules.

- Runtime          (system coordination)
- Scheduler        (execution selection)
- Task             (task lifecycle)
- Context          (execution state management)
- Memory           (allocation abstractions)
- Platform         (platform-specific functionality)
- Common           (shared utilities)
- Data Structures  (internal reusable containers)

Each module owns a single responsibility and exposes only the interfaces required by higher layers.

---

# Public API

The public API shall remain intentionally small.

Users should only interact with the runtime through headers under:

include/gt/

Everything under:

internal/gt\_internal/

is considered private implementation.

---

# Scheduler

Version 1 uses:

- one scheduler
- one run queue
- FIFO scheduling
- cooperative yielding

No scheduler decisions depend on priorities or time slices.

---

# Task Model

Each task owns:

- execution context
- stack
- lifecycle state
- unique identifier

Tasks execute until:

- they yield
- they return
- they terminate because of a fatal runtime error

---

# Context Switching

Version 1 uses:

ucontext

as the only supported backend.

The backend is isolated behind the context subsystem so it can be replaced in future versions without affecting the scheduler or runtime.

---

# Memory Model

Each task owns one stack allocated by the runtime.

Stacks are allocated dynamically.

Stacks are released when the task is destroyed.

Memory ownership must always be explicit.

---

# Error Handling

Recoverable errors return explicit status codes.

Unrecoverable internal errors terminate through the panic subsystem.

Undefined behavior must never be relied upon.

---

# Coding Standards

The project follows consistent naming, formatting, and architectural conventions.

These standards are defined separately and apply to all source files.

---

# Success Criteria

Version 1 is considered complete when all of the following are true.

- runtime initializes successfully
- tasks can be created
- tasks execute correctly
- context switching works reliably
- yielding schedules another runnable task
- completed tasks are destroyed
- memory is released correctly
- tests pass
- examples execute correctly
- benchmarks run successfully

No additional functionality is required for Version 1.

---

# Future Versions

Future releases may introduce:

- setjmp()/longjmp()
- assembly context switching
- stack pools
- guard pages
- worker threads
- work stealing
- preemptive scheduling
- timers
- epoll reactor
- asynchronous I/O
- lock-free scheduling

These features are explicitly outside the Version 1 contract.

---

End of Contract.
