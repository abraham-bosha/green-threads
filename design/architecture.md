# Runtime Architecture

Version: 1.0

Status: Frozen

---

# Purpose

This document defines the high-level architecture of the green\_threads runtime.

It specifies the responsibilities, boundaries, ownership, and interactions of every subsystem.

Implementation details are intentionally omitted.

This document is normative.

If implementation conflicts with this document, the implementation is incorrect.

---

# Architectural Goals

The runtime is designed to be

- simple
- modular
- predictable
- extensible
- testable

The architecture favors correctness over optimization.

Future versions should be able to replace individual modules without redesigning the entire runtime.

---

# Architectural Principles

## Single Responsibility

Every subsystem owns one responsibility.

Subsystems should not perform work belonging to another subsystem.

---

## Explicit Ownership

Every resource has one owner.

Ownership transfers must be explicit.

Shared ownership is avoided.

---

## Layered Design

Dependencies always point downward.

Circular dependencies are forbidden.

---

## Encapsulation

Implementation details remain private.

Public interfaces expose only stable abstractions.

---

## Replaceable Components

Subsystems should be replaceable whenever practical.

Examples:

- ucontext → setjmp
- setjmp → assembly
- simple stack allocator → stack pool

without affecting unrelated modules.

---

# Dependency Rules

Runtime may depend on

- Scheduler
- Task
- Context
- Memory
- Platform
- Common
- Data Structures

Scheduler may depend on

- Task
- Common
- Data Structures

Task may depend on

- Context
- Memory
- Common

Context may depend on

- Platform
- Common

Memory may depend on

- Platform
- Common

Platform may depend on

- Common

Data Structures depend only on 

- Common

Common depends on nothing.

---

# Runtime Overview

The runtime consists of the following modules.

- Runtime
- Scheduler
- Task
- Context
- Memory
- Platform
- Common
- Data Structures

Each module has a well-defined responsibility.

---

# Runtime

The runtime is the root of the system.

Responsibilities

- initialization
- shutdown
- configuration
- public API entry points
- subsystem coordination

The runtime owns every subsystem.

It is the only module responsible for starting and stopping the system.

---

# Scheduler

The scheduler determines which task executes next.

Responsibilities

- maintain runnable queue
- enqueue tasks
- dequeue tasks
- dispatch tasks
- process cooperative yield

Version 1 implements

- FIFO scheduling
- one scheduler
- one run queue

The scheduler never allocates memory.

The scheduler never performs context switching directly.

---

# Task

A task represents one execution unit.

Responsibilities

- lifecycle
- task state
- task identifier
- context ownership
- stack ownership

Every task owns

- one execution context
- one stack
- one unique identifier

Tasks never schedule themselves.

---

# Context

The context subsystem abstracts CPU execution state.

Responsibilities

- create context
- destroy context
- switch context

Version 1 uses

ucontext

as the backend.

The scheduler never depends on backend implementation details.

---

# Memory

The memory subsystem manages runtime-owned memory.

Responsibilities

- allocate stacks
- release stacks

Version 1 performs simple dynamic allocation.

Future versions may introduce

- stack pools
- guard pages
- custom allocators

without changing higher-level modules.

---

# Platform

The platform subsystem isolates operating-system interfaces.

Responsibilities

- mmap
- page information

The rest of the runtime never calls operating-system APIs directly.

All platform-specific functionality passes through this module.

---

# Common

The common module provides shared utilities.

Responsibilities

- assertions
- panic handling
- compiler abstractions
- project types
- common macros

The common module owns no runtime state.

---

# Data Structures

Generic data structures remain independent from the runtime.

Version 1 includes

- queue
- list
- bitmap
- id\_pool

These structures contain no scheduler-specific or task-specific behavior.

They are reusable by future projects.

---

# Ownership Model

Runtime owns

- scheduler
- runtime configuration
- runtime state

Scheduler owns

- run queue

Task owns

- context
- stack
- identifier

Memory owns

- stack allocation

Platform owns

- operating-system interaction

Ownership is never ambiguous.

---

# Execution Flow

Runtime initializes.

     ↓

Scheduler initializes.

     ↓

Task is created.

     ↓

Stack is allocated.

     ↓

Context is created.

     ↓

Task enters run queue.

     ↓ 

Scheduler dispatches task.

     ↓

Context switch occurs.

     ↓

Task executes.

     ↓

Task yields or exits.

     ↓

Scheduler resumes another task.

     ↓

Task resources are released.

     ↓

Runtime shuts down.

---

# Context Switching

The scheduler chooses the next runnable task.

The context subsystem performs the actual switch.

The scheduler must never manipulate CPU registers directly.

Backend implementation details remain isolated.

---

# Error Handling

Recoverable failures return status codes.

Fatal runtime failures terminate through the panic subsystem.

Subsystems do not silently ignore failures.

---

# Extensibility

The architecture is intentionally prepared for future extensions.

Examples include

- setjmp backend
- assembly backend
- worker threads
- work stealing
- timers
- reactor
- asynchronous I/O

These extensions should require adding modules rather than redesigning existing ones.

---

# Non-Goals

Version 1 does not support

- preemptive scheduling
- multiple schedulers
- worker threads
- synchronization primitives
- asynchronous I/O
- lock-free scheduling
- custom memory allocators

---

# Architectural Stability

Version 1 freezes

- subsystem boundaries
- ownership rules
- dependency direction
- layering
- module responsibilities

Implementation details may evolve.

The architecture should remain stable.

---

End of Document.
