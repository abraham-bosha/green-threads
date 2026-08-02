# Scheduler Contract

## Overview

The Scheduler subsystem is responsible for managing the set of runnable tasks and selecting the next task eligible for execution according to the runtime scheduling policy.

The Scheduler owns scheduling policy but does not own task execution, context switching, task lifecycle management, or runtime control flow.

---

# Design Principles

## Policy-Centric Design

The Scheduler is responsible only for deciding which runnable task should execute next.

It does not perform execution itself.

---

## Runtime-Orchestrated Execution

The Runtime owns execution control.

The Scheduler participates only by responding to runtime requests for scheduling decisions.

---

## Policy Independent of Mechanism

The Scheduler defines scheduling policy.

It never depends on or exposes implementation details of context switching, task execution, or memory management.

---

## Container Independence

The Scheduler exposes scheduling operations rather than queue operations.

The internal data structure used to maintain runnable tasks is an implementation detail and may change without affecting higher-level components.

---

# Responsibilities

The Scheduler subsystem is responsible for:

- Maintaining the set of runnable tasks.
- Admitting runnable tasks.
- Selecting the next runnable task.
- Applying the scheduling policy.
- Maintaining the ordering of runnable tasks.

---

# Non-Responsibilities

The Scheduler subsystem does not own or manage:

- Runtime lifecycle
- Task creation
- Task destruction
- Task identifiers
- Task execution
- Task state transitions
- Context switching
- Execution contexts
- Stack allocation
- Memory management
- Synchronization primitives
- Worker threads

These responsibilities belong to other subsystems.

---

# Ownership

The Scheduler owns the runnable-task collection.

The Scheduler does not own the tasks contained within that collection.

Tasks are owned by the Runtime.

The Scheduler stores only references to runnable tasks through intrusive container linkage.

---

# Dependencies

The Scheduler subsystem may depend on:

- Common utility modules
- Intrusive list
- Queue
- Task

The Scheduler shall never depend on:

- Runtime
- Platform Context
- Platform VM
- Memory
- Synchronization
- Higher-level runtime components

Dependency flow shall always move downward.

---

# Lifecycle

The Scheduler follows a simple lifecycle.

```text
Uninitialized
        │
        ▼
Initialize
        │
        ▼
Initialized
        │
        ├──────────────────────────────┐
        ▼                              │
Admit Runnable Task                    │
        │                              │
        ▼                              │
Select Runnable Task                   │
        │                              │
        └──────────────────────────────┘
        │
        ▼
Destroy
        │
        ▼
Uninitialized
```

The Scheduler remains initialized until it is explicitly destroyed.

---

# Scheduling Policy

Version 1 implements a cooperative Round Robin scheduling policy.

Runnable tasks are maintained in FIFO order.

When a task becomes runnable, it is admitted to the back of the runnable queue.

When the Runtime requests the next task, the Scheduler selects and removes the task at the front of the runnable queue.

The Scheduler never preempts running tasks.

---

# Invariants

## Runnable Tasks Only

Every task contained in the Scheduler shall be in the `READY` state.

Tasks in any other state shall never be admitted.

---

## Unique Membership

A runnable task shall appear at most once within the Scheduler.

Duplicate admission of the same task is prohibited.

---

## No Task Ownership

The Scheduler never owns, allocates, initializes, or destroys tasks.

It manages only the runnable-task collection.

---

## Stable Task Identity

The Scheduler shall never modify:

- Task identifier
- Entry function
- Entry argument
- Stack
- Execution context

These properties belong to the Task subsystem.

---

## FIFO Ordering

Version 1 preserves FIFO ordering for runnable tasks.

Tasks are always selected in the order determined by the Round Robin policy.

---

## Selection Removes Membership

Selecting a runnable task removes it from the runnable-task collection.

A running task shall not simultaneously remain in the Scheduler.

---

## Empty Scheduler

An empty Scheduler is a valid state.

Requesting the next runnable task from an empty Scheduler shall return no task.

---

## No Task State Management

The Scheduler never changes task states.

Task state transitions are performed exclusively by the Runtime.

---

## No Context Switching

The Scheduler never performs execution-context switching.

Execution transfer belongs to the Runtime through the Platform Context subsystem.

---

## Policy Only

The Scheduler determines which runnable task should execute next.

It never determines:

- How execution is transferred
- How tasks are created
- How stacks are allocated
- How memory is managed

---

# Runtime Interaction

The Runtime exclusively drives the Scheduler.

Typical interaction follows the sequence:

```text
Task becomes READY
        │
        ▼
Runtime updates task state
        │
        ▼
Scheduler admits task
        │
        ▼
Runtime requests next task
        │
        ▼
Scheduler selects runnable task
        │
        ▼
Runtime performs context switch
```

The Scheduler never invokes Runtime operations.

---

# Error Handling

Programmer contract violations are detected through internal assertions.

Examples include:

- Null Scheduler pointer
- Null task pointer
- Admission of a non-runnable task
- Duplicate task admission

Normal runtime conditions are not considered errors.

Requesting the next runnable task from an empty Scheduler returns no task.

---

# Interface Philosophy

The Scheduler exposes only scheduling operations.

Its interface is intentionally minimal and consists solely of operations required to:

- Initialize the Scheduler.
- Admit runnable tasks.
- Select the next runnable task.
- Destroy the Scheduler.

The Scheduler performs scheduling policy—not execution.

---

End of Document.
