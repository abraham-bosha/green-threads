# Memory Subsystem

## Overview

The Memory subsystem provides the runtime with execution stacks.

An execution stack is a runtime memory object that owns the virtual memory required for task execution and presents a valid stack region to the Context subsystem.

The subsystem transforms generic virtual memory mappings into execution-ready stacks while hiding all platform-specific memory management details.

The Memory subsystem owns stack construction and lifetime but does not own task execution, scheduling, or context switching.

---

## Design Principles

### Runtime-Centric Abstractions

The subsystem exposes runtime execution stacks rather than operating-system memory primitives.

Platform-specific concepts such as virtual memory mappings, page protection, and guard pages remain implementation details.

---

### Stack-Oriented Interface

The interface describes the lifecycle of execution stacks.

The subsystem is responsible only for creating and destroying execution stacks.

It does not expose low-level virtual memory operations.

---

### Layered Architecture

The Memory subsystem builds upon the Platform VM subsystem.

Higher-level runtime components depend only on execution stacks and remain unaware of the underlying virtual memory implementation.

---

## Responsibilities

The Memory subsystem is responsible for:

- Representing execution stacks.
- Initializing execution stacks.
- Destroying execution stacks.
- Reserving virtual memory through the Platform VM subsystem.
- Constructing the execution stack layout.
- Establishing and maintaining guard pages.
- Exposing the usable execution stack region.
- Owning the lifetime of virtual memory mappings backing execution stacks.

---

## Non-Responsibilities

The Memory subsystem does not own or manage:

- Tasks
- Task identifiers
- Task states
- Scheduling policy
- Ready queues
- Worker threads
- Execution contexts
- Context switching
- Register state
- Synchronization primitives
- Virtual memory implementation

These responsibilities belong to other subsystems.

---

## Ownership

Execution stacks are owned by the runtime objects that contain them.

The Memory subsystem defines and manipulates execution stacks but does not own the runtime objects that contain them.

Each execution stack exclusively owns its backing virtual memory mapping.

The Memory subsystem acquires and releases the backing mapping through the Platform VM subsystem.

---

## Dependencies

The Memory subsystem may depend on:

- Common utility modules
- Platform Page
- Platform VM

The subsystem shall never depend on:

- Task
- Context
- Scheduler
- Synchronization
- Higher-level runtime components

Dependency flow shall always move downward.

---

## Lifecycle

Every execution stack follows the same lifecycle.

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
Destroy
        │
        ▼
Uninitialized
```

Initialization occurs exactly once.

After destruction, the stack may be initialized again as a new execution stack.

---

## Invariants

### Ownership

An execution stack has exactly one owner.

Execution stacks shall never be shared between runtime objects.

---

### Single Mapping

Every execution stack owns exactly one virtual memory mapping.

The mapping exists exclusively for the lifetime of that execution stack.

---

### Stable Address

Once initialized, the usable stack region shall remain at a stable virtual address until the stack is destroyed.

---

### Valid Stack Region

The usable execution stack shall:

- Reside entirely within its backing virtual memory mapping.
- Satisfy all platform alignment requirements.
- Remain valid for the lifetime of the execution stack.

---

### Immutable Layout

The following properties remain immutable after initialization:

- Stack base
- Stack size
- Stack layout
- Guard-page placement

Only execution state changes during task execution.

---

### Guard Pages

Guard pages shall remain inaccessible for the lifetime of the execution stack.

Their existence and placement are implementation details of the Memory subsystem.

---

### Initialization Before Use

Only initialized execution stacks may be supplied to the Context subsystem.

Passing an uninitialized execution stack constitutes a programmer contract violation.

---

### No Execution Ownership

The Memory subsystem never:

- Executes tasks.
- Performs context switching.
- Preserves register state.
- Controls execution flow.

These responsibilities belong exclusively to the Context subsystem.

---

### Platform Encapsulation

Higher-level runtime components shall never depend on:

- Virtual memory mappings
- Page protection
- Operating-system memory APIs
- Platform-specific allocation mechanisms

These implementation details remain encapsulated by the Memory subsystem.

---

## Platform VM Contract

The Memory subsystem acquires virtual memory exclusively through the Platform VM subsystem.

The Platform VM subsystem provides the mechanisms for reserving, protecting, and destroying virtual memory mappings.

The Memory subsystem defines how those mappings are organized into execution stacks.

---

## Error Handling

Programmer contract violations are detected through internal assertions.

Operational failures originating from the Platform VM subsystem are translated into runtime status codes through the Memory subsystem interface.

---

## Interface Philosophy

The Memory subsystem exposes only execution-stack operations.

Its interface is intentionally minimal and consists solely of operations required to:

- Initialize an execution stack.
- Destroy an execution stack.

Virtual memory management, page protection, guard pages, and stack layout remain internal implementation details.

The Memory subsystem performs execution-stack management—not execution or virtual memory management.

---

End of Document
