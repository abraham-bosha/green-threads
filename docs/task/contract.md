# Task Subsystem Contract

## Overview

The Task subsystem defines the runtime representation of an executable task.

A task is a runtime object that owns all resources required to execute a single unit of work, including its execution stack, execution context, entry function, and execution state.

The subsystem is responsible for constructing and destroying executable tasks while remaining independent of scheduling policy and platform-specific execution mechanisms.

---

# Design Principles

## Resource Composition

A task is a composition of lower-level runtime resources.

The Task subsystem assembles these resources into a single executable runtime object but does not implement the underlying resource management itself.

---

## Single Ownership

Every task exclusively owns its execution resources.

A task owns exactly one:

- Execution stack
- Execution context
- Entry function
- Entry argument
- Execution state

Task identifiers are assigned by the Runtime and stored by the Task subsystem.

---

## Separation of Responsibilities

The Task subsystem is responsible only for task construction, destruction, and state representation.

Execution scheduling, synchronization, waiting, and runtime policy belong to other subsystems.

---

# Responsibilities

The Task subsystem is responsible for:

- Representing executable runtime tasks.
- Initializing task objects.
- Destroying task objects.
- Owning execution stacks.
- Owning execution contexts.
- Storing task identifiers.
- Storing task entry functions.
- Storing task entry arguments.
- Maintaining task execution state.

---

# Non-Responsibilities

The Task subsystem does not own or manage:

- Task identifier allocation
- Task scheduling
- Scheduling policy
- Ready queues
- Worker threads
- Context switching
- Stack allocation mechanisms
- Virtual memory management
- Synchronization primitives
- Sleeping or waiting
- Runtime lifecycle management

These responsibilities belong to their respective subsystems.

---

# Ownership

Task objects are owned by the Runtime.

The Runtime is responsible for allocating task storage, assigning task identifiers, and destroying task objects.

The Task subsystem owns the resources contained within a task object but does not own the lifetime of the task object itself.

Execution stacks are owned by each task and managed through the Memory subsystem.

Execution contexts are owned by each task and managed through the Platform Context subsystem.

---

# Dependencies

The Task subsystem may depend on:

- Common utility modules
- Memory
- Platform Context

The Task subsystem shall never depend on:

- Scheduler
- Higher-level runtime components

Dependency flow shall always move downward.

---

# Lifecycle

Every task follows the same lifecycle.

```text
Uninitialized
        │
        ▼
Initialize
        │
        ▼
NEW
        │
        ▼
READY
        │
        ▼
RUNNING
        │
        ▼
EXITED
        │
        ▼
Destroy
        │
        ▼
Uninitialized
```

Task initialization constructs a complete executable task.

Task destruction releases every resource owned by the task.

---

# State Model

## NEW

The task has been successfully initialized but has not yet been made runnable.

The task owns all execution resources but has never executed.

---

## READY

The task is eligible for execution.

The scheduler may select the task for execution.

---

## RUNNING

The task is currently executing.

Its execution context is active and its execution stack is in use.

---

## COMPLETED

The task has finished executing.

A completed task shall never become runnable again.

---

# Invariants

## Stable Address

A task object shall remain at a stable memory address throughout its lifetime.

---

## Single Ownership

Every execution stack belongs to exactly one task.

Every execution context belongs to exactly one task.

Execution resources shall never be shared between tasks.

---

## Immutable Configuration

The following task configuration remains immutable after successful initialization:

- Task identifier
- Entry function
- Entry argument
- Execution stack
- Execution context

Only the execution state changes during the lifetime of a task.

---

## Valid Execution Resources

A successfully initialized task always owns:

- A valid execution stack
- A valid execution context

A partially initialized task shall never be exposed outside the Task subsystem.

---

## No Scheduling Policy

The Task subsystem never determines:

- When a task executes.
- Which task executes next.
- Why execution switches occur.

Scheduling decisions belong exclusively to the Scheduler.

---

## Runtime-Owned Identity

Task identifiers are assigned by the Runtime before task initialization.

The Task subsystem stores the assigned identifier but never allocates or releases task identifiers.

---

## Resource Cleanup

Every resource successfully acquired during task initialization shall be released during task destruction.

Initialization failures shall roll back all previously acquired resources before returning.

---

# Initialization Contract

Task initialization shall:

1. Validate input parameters.
2. Initialize the execution stack.
3. Initialize the execution context.
4. Store the assigned task identifier.
5. Store the entry function.
6. Store the entry argument.
7. Set the initial task state.
8. Return an appropriate status code.

Initialization is atomic from the caller's perspective.

If initialization fails, no resources owned by the task shall remain allocated.

---

# Destruction Contract

Task destruction shall:

1. Destroy the execution context.
2. Destroy the execution stack.
3. Reset the task object to its uninitialized state.

Task destruction assumes ownership only of resources acquired by successful task initialization.

---

# Error Handling

Programmer contract violations are detected through internal assertions.

Operational failures originating from dependent subsystems are translated into runtime status codes and propagated to the caller.

---

# Interface Philosophy

The Task subsystem exposes only object lifecycle operations.

Its interface is intentionally minimal and consists solely of operations required to:

- Initialize a task.
- Destroy a task.

Task scheduling, execution, synchronization, and runtime management belong exclusively to higher-level subsystems.

The Task subsystem constructs executable tasks—it does not execute them.

---

End of Document
