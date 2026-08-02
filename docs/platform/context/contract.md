# Platform Context Contract

## Overview

The platform context subsystem provides higher-level subsystems with an abstraction for creating, preserving, restoring, and destroying execution contexts.

An execution context represents a resumable execution state. it encapsulates the operating-system and architecture-specific mechanisms required to suspend one execution flow and resume another while exposing a stable, runtime-oriented interface.

The subsystem owns the execution-context abstraction but does not own scheduling policy, task management, or memory management.

---

## Design Principles

### Runtime-centric Abstractions

The subsystem exposes only the execution-context concepts required by the runtime rather than the complete capabilities of the underlying operating system or implementation.

Backend-specific details remain entirely encapsulated.

---

### Execution-centric Interfaces

The interface describes execution mechanics rather than runtime policy.

The subsystem is responsible for creating, preserving, restoring, and destroying execution contexts. it is not responsible for deciding when or why execution switches occur.

---

### Backend Replaceability

Backend implementations are interchangeable.

Replacing one backend with another shall not require modifications to the platform context interface or to any higher-level subsystem.

- runtime components
- scheduler components
- higher platform interfaces

Only the backend implementation itself should change.

---

## Responsibilities

The platform context subsystem is responsible for:

- representing execution contexts.
- initializing execution contexts.
- configuring initial execution state.
- preserving execution state.
- restoring execution state.
- destroying execution contexts.
- encapsulating platform-specific context-switching mechanisms.

---

## Non-responsibilities

The platform context subsystem does not own or manage:

- tasks
- task identifiers
- task states
- scheduling policy
- ready queues
- worker threads
- stack allocation
- stack pools
- guard pages
- virtual memory management
- synchronization primitives

These responsibilities belong to other subsystems.

---

## Ownership

Execution contexts are owned by the runtime.

The platform context subsystem defines and manipulates execution contexts but does not own the runtime objects that contain them.

Execution stacks are owned by the memory subsystem.

The platform context subsystem consumes stack memory supplied by the caller but never allocates or releases stacks itself.

---

## Dependencies

The platform context subsystem may depend on:

- common utility modules
- platform page
- memory subsystem (stack allocation)
- backend implementation

The subsystem shall never depend on:

- runtime
- scheduler
- task management
- synchronization
- higher-level runtime components

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
        ▼
Configure
        │
        ▼
Configured
        │
        ├──────────────┐
        ▼              │
Context Switch         │
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

Execution contexts are embedded within runtime-owned task objects. The Platform Context subsystem operates on execution contexts but never owns the objects that contain them.

Execution contexts shall never be shared between runtime objects.

---

### Stable Address

Once initialized, an execution context shall remain at a stable memory address until it is destroyed.

---

### Single Initialization

A context shall not be initialized more than once without first being destroyed.

---

### Single Configuration

An initialized execution context shall be configured at most once before execution begins.

Reconfiguring an execution context that has already been configured is undefined.

---

### Valid Stack

Context initialization requires a valid execution stack.

The supplied stack shall satisfy all platform alignment and size requirements.

---

### Immutable Configuration

The following execution-context configuration becomes immutable after context configuration:

- entry function
- entry argument
- stack

Context switching modifies only the saved execution state.

---

### Initialized Contexts

Only initialized execution contexts may participate in context switching.

---

### No Scheduling Policy

The subsystem never determines:

- which context executes next.
- why a context switch occurs.
- when execution switches occur.

These decisions belong exclusively to the scheduler.

---

### No Memory Ownership

The subsystem never allocates or releases execution stacks.

It only operates on caller-supplied stack memory.

---

### Backend Transparency

Higher-level components shall never depend on backend-specific implementation details.

Backend-specific data structures and mechanisms remain entirely encapsulated by the platform context subsystem.

---

## Backend Contract

Every backend implementation shall:

- implement the platform context interface completely.
- preserve execution semantics.
- encapsulate all backend-specific details.
- expose no native implementation details upward.
- depend only on lower-level operating-system or architecture facilities.
- preserve the platform ABI.

---

## Error Handling

Programmer contract violations are detected through internal assertions.

Backend-specific failures are translated into status codes before crossing the platform context interface.

---

## Interface Philosophy

The platform context subsystem exposes only execution-context operations.

Its interface is intentionally minimal and consists solely of operations required to:

- initialize
- configure
- save
- load
- destroy

Additional functionality belongs only in the subsystem that owns the corresponding responsibility.

The platform context subsystem performs execution mechanics—not runtime policy.

---

End of Document.
