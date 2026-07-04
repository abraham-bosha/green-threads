# Runtime Invariants

Version: 1.0

Status: Frozen

---

# Purpose

This document defines the invariants of the green\_threads runtime.

An invariant is a condition that must always remain true during correct operation of the runtime.

Implementation details may change.

Architectural boundaries may evolve in future versions.

These invariants must remain valid unless explicitly revised.

If implementation violates an invariant, the implementation is incorrect.

---

# Design Goals

The invariant system exists to guarantee:

* correctness
* predictability
* maintainability
* debuggability
* architectural integrity

---

# General Invariants

The runtime must always satisfy the following conditions.

* ownership is explicit
* resources have a single owner
* circular dependencies never exist
* subsystem boundaries remain respected
* runtime state remains internally consistent

Undefined behavior must never be relied upon.

---

# Runtime Invariants

The runtime is the root of the system.

The following conditions must always hold.

* the runtime must be initialized before use
* the runtime must not be initialized more than once
* the runtime must remain valid while tasks exist
* runtime shutdown must occur only after all runtime-owned resources are released
* runtime state must remain internally consistent

Version 1 supports exactly one runtime instance.

---

# Runtime State Invariants

Version 1 runtime states are:

- UNINITIALIZED
- INITIALIZED
- RUNNING
- SHUTDOWN

Valid transitions:

UNINITIALIZED
    ↓
INITIALIZED
    ↓
RUNNING
    ↓
SHUTDOWN

The following conditions must always hold:

- Runtime starts in UNINITIALIZED state.
- Runtime must be initialized before entering RUNNING state.
- Runtime may not return to a previous state.
- SHUTDOWN is terminal.
- Tasks may only be created while runtime is RUNNING.

---

# Scheduler Invariants

The scheduler controls task execution order.

The following conditions must always hold.

* only runnable tasks may exist in the run queue
* a task may appear in the run queue at most once
* FIFO ordering must be preserved
* the scheduler must never own task stacks
* the scheduler must never own task contexts
* the scheduler must never perform context switching directly

The scheduler selects tasks.

The context subsystem performs execution transfer.

---

# Task Invariants

A task represents a single execution unit.

The following conditions must always hold.

* every task has a unique identifier
* every task owns exactly one stack
* every task owns exactly one execution context
* every task exists in exactly one lifecycle state
* task ownership is never shared

A task must never exist simultaneously in multiple states.

---

# Task State Invariants

Version 1 task states are:

* CREATED
* RUNNABLE
* RUNNING
* COMPLETED

Valid transitions:

       ┌───────────┐
       │  CREATED  │
       └─────┬─────┘
             │ (admit)
             ▼
       ┌───────────┐
       │  RUNNABLE │ ◄───┐
       └─────┬─────┘     │
             │           │
             │(dispatch) │ (yield)
             ▼           │
       ┌───────────┐     │
       │  RUNNING  ├─────┘
       └─────┬─────┘
             │
             │ (exit)
             ▼
       ┌───────────┐
       │ COMPLETED │
       └───────────┘

The following conditions must always hold.

* a task may transition only through valid state transitions
* a completed task must never become runnable again
* a running task must be the currently executing task
* a task must never execute before becoming runnable

Invalid state transitions are runtime errors.

---

# Context Invariants

The context subsystem manages execution state.

The following conditions must always hold.

* every context belongs to exactly one task
* a context must be initialized before use
* context switching occurs only through the context subsystem
* the scheduler must never manipulate execution state directly
* context ownership is never shared

The context subsystem does not own task stacks.

---

# Memory Invariants

The memory subsystem manages runtime-owned memory.

The following conditions must always hold.

* every allocated stack has exactly one owner
* every allocated stack is eventually released
* stack ownership is explicit
* memory ownership is never shared
* memory must never be released twice

A released stack must never be accessed again.

---

# Platform Invariants

The platform subsystem isolates operating-system interaction.

The following conditions must always hold.

* operating-system interaction occurs only through the platform subsystem
* higher-level modules must never invoke operating-system APIs directly
* platform functionality remains isolated from runtime logic

The platform subsystem does not contain scheduling logic.

---

# Data Structure Invariants

Data structures remain independent from runtime behavior.

The following conditions must always hold.

* data structures remain runtime-agnostic
* data structures contain no scheduler-specific behavior
* data structures contain no task-specific behavior
* data structures contain no context-specific behavior

Reusable data structures must not depend on runtime modules.

---

# Dependency Invariants

The dependency graph must always remain valid.

The following conditions must always hold.

* dependencies flow downward
* circular dependencies are forbidden
* modules may depend only on approved dependencies
* implementation details must not bypass architectural layers

The dependency graph must remain a Directed Acyclic Graph (DAG).

---

# Ownership Invariants

Ownership rules are fundamental to the runtime.

The following conditions must always hold.

* every resource has exactly one owner
* ownership transfers are explicit
* ownership is never ambiguous
* resources are released by their owner

Shared ownership is not permitted in Version 1.

---

# Error Handling Invariants

The runtime must never silently ignore failures.

The following conditions must always hold.

* recoverable errors return status codes
* fatal errors terminate through the panic subsystem
* invalid states must not be ignored
* invariant violations indicate bugs

Correctness takes priority over continued execution.

---

# Architectural Invariants

The architecture defined by architecture.md must remain valid.

The following conditions must always hold.

* subsystem responsibilities remain respected
* subsystem boundaries remain respected
* ownership rules remain respected
* dependency rules remain respected

Implementation may evolve.

Architectural invariants remain stable.

---

# Stability

These invariants are frozen for Version 1.

Any proposed change requires architectural review.

---

End of Document.
