# Platform Context Contract

## Overview

The Platform Context subsystem provides the runtime with an abstraction for creating, preserving, restoring, and destroying execution contexts.

An execution context represents a resumable execution state. It encapsulates the operating-system and architecture-specific mechanisms required to suspend one execution flow and resume another while exposing a stable, runtime-oriented interface.

The subsystem owns the execution-context abstraction but does not own scheduling policy, task management, or memory management.

---

## Design Principles

### Runtime-Centric Abstractions

The subsystem exposes only the execution-context concepts required by the runtime rather than the complete capabilities of the underlying operating system or implementation.

Backend-specific details remain entirely encapsulated.

---

### Execution-Centric Interfaces

The interface describes execution mechanics rather than runtime policy.

The subsystem is responsible for creating, preserving, restoring, and destroying execution contexts. It is not responsible for deciding when or why execution switches occur.

---

### Backend Replaceability

Backend implementations are interchangeable.

Replacing one backend with another shall not require modifications to the Platform Context interface or to any higher-level subsystem.

- Runtime components
- Scheduler components
- Higher platform interfaces

Only the backend implementation itself should change.

---

## Responsibilities

The Platform Context subsystem is responsible for:

- Representing execution contexts.
- Initializing execution contexts.
- Preserving execution state.
- Restoring execution state.
- Destroying execution contexts.
- Encapsulating platform-specific context-switching mechanisms.

---

## Non-Responsibilities

The Platform Context subsystem does not own or manage:

- Tasks
- Task identifiers
- Task states
- Scheduling policy
- Ready queues
- Worker threads
- Stack allocation
- Stack pools
- Guard pages
- Virtual memory management
- Synchronization primitives

These responsibilities belong to other subsystems.

---

## Ownership

Execution contexts are owned by the runtime.

The Platform Context subsystem defines and manipulates execution contexts but does not own the runtime objects that contain them.

Execution stacks are owned by the Memory subsystem.

The Platform Context subsystem consumes stack memory supplied by the caller but never allocates or releases stacks itself.

---

## Dependencies

The Platform Context subsystem may depend on:

- Common utility modules
- Platform Page
- Platform Memory (stack abstraction)
- Backend implementation

The subsystem shall never depend on:

- Runtime
- Scheduler
- Task management
- Synchronization
- Higher-level runtime components

Dependency flow shall always move downward.

---

## Lifecycle

Every execution context follows the same lifecycle.

```text
Uninitialized
        │
        ▼
Initialize
        │
        ▼
Initialized
        │
        ├──────────────┐
        ▼              │
Switch Context         │
        ▲              │
        └──────────────┘
        │
        ▼
Destroy
        │
        ▼
Uninitialized
```

Initialization occurs exactly once.

After destruction, the context may be initialized again as a new execution context.

---

## Invariants

### Ownership

An execution context has exactly one owner.

Execution contexts shall never be shared between runtime objects.

---

### Stable Address

Once initialized, an execution context shall remain at a stable memory address until it is destroyed.

---

### Single Initialization

A context shall not be initialized more than once without first being destroyed.

---

### Valid Stack

Context initialization requires a valid execution stack.

The supplied stack shall satisfy all platform alignment and size requirements.

---

### Immutable Configuration

The following execution-context configuration remains immutable after initialization:

- Entry function
- Entry argument
- Stack

Context switching modifies only the saved execution state.

---

### Initialized Contexts

Only initialized execution contexts may participate in context switching.

---

### No Scheduling Policy

The subsystem never determines:

- Which context executes next.
- Why a context switch occurs.
- When execution switches occur.

These decisions belong exclusively to the scheduler.

---

### No Memory Ownership

The subsystem never allocates or releases execution stacks.

It only operates on caller-supplied stack memory.

---

### Backend Transparency

Higher-level components shall never depend on backend-specific implementation details.

Backend-specific data structures and mechanisms remain entirely encapsulated by the Platform Context subsystem.

---

## Backend Contract

Every backend implementation shall:

- Implement the Platform Context interface completely.
- Preserve execution semantics.
- Encapsulate all backend-specific details.
- Expose no native implementation details upward.
- Depend only on lower-level operating-system or architecture facilities.

Backend implementations are free to use any mechanism necessary to implement execution contexts, including:

- POSIX ucontext
- setjmp/longjmp
- Handwritten assembly
- Platform-native context facilities

Provided the externally observable behavior remains identical.

---

## Error Handling

Programmer contract violations are detected through internal assertions.

Backend-specific failures are translated into runtime status codes before crossing the Platform Context interface.

---

## Interface Philosophy

The Platform Context subsystem exposes only execution-context operations.

Its interface is intentionally minimal and consists solely of operations required to:

- Initialize an execution context.
- Switch between execution contexts.
- Destroy an execution context.

Additional functionality belongs only in the subsystem that owns the corresponding responsibility.

The Platform Context subsystem performs execution mechanics—not runtime policy.

---

End of Document
