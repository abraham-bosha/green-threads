# Runtime Architecture

**Version:** 1.0

**Status:** Draft

---

# Purpose

This document defines the high-level architecture of the green-threads runtime.

It specifies the responsibilities, ownership, boundaries, and interactions of every subsystem.

Implementation details are intentionally omitted.

This document is normative.

If the implementation conflicts with this document, the implementation is incorrect.

---

# Architectural Goals

The runtime is designed to be

* simple
* modular
* predictable
* extensible
* testable

The architecture favors correctness over optimization.

Future versions should be able to replace individual subsystems without redesigning the entire runtime.

---

# Architectural Principles

## Single Responsibility

Every subsystem owns exactly one responsibility.

Subsystems must not perform work belonging to another subsystem.

---

## Explicit Ownership

Every resource has exactly one owner.

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

Examples include

* assembly → setjmp
* setjmp → ucontext
* simple stack allocator → stack pool

without affecting unrelated modules.

---

# Dependency Rules

Runtime may depend on

* Scheduler
* Task
* Context
* Memory
* Platform
* Common
* Data Structures

Scheduler may depend on

* Task
* Common
* Data Structures

Task may depend on

* Context
* Memory
* Common

Context may depend on

* Platform
* Common

Memory may depend on

* Platform
* Common

Platform may depend on

* Common

Data Structures may depend on

* Common

Common depends on nothing.

---

# Runtime Overview

The runtime consists of the following modules.

* Runtime
* Scheduler
* Task
* Context
* Memory
* Platform
* Common
* Data Structures

Each module has a clearly defined responsibility.

---

# Runtime

The runtime is the root of the system.

Responsibilities

* initialization
* shutdown
* public API entry points
* subsystem coordination
* execution control

The runtime owns every subsystem.

It is the only module responsible for starting and stopping the system.

---

# Scheduler

The scheduler determines which runnable task executes next.

Responsibilities

* maintain the runnable queue
* enqueue runnable tasks
* dequeue runnable tasks
* select the next runnable task

Version 1 implements

* FIFO scheduling
* one scheduler
* one runnable queue

The scheduler never allocates memory.

The scheduler never performs context switching.

---

# Task

A task represents one execution unit.

Responsibilities

* task initialization
* task destruction
* task metadata
* execution context ownership
* stack ownership

Every task owns

* one execution context
* one stack
* one unique identifier

Tasks never schedule themselves.

---

# Context

The context subsystem abstracts CPU execution state.

Responsibilities

* initialize execution contexts
* configure initial execution state
* save execution state
* restore execution state

Version 1 uses a dedicated x86-64 assembly backend.

Higher-level subsystems never depend on backend implementation details.

---

# Memory

The memory subsystem manages runtime-owned memory.

Responsibilities

* allocate stacks
* guard pages
* release stacks

Version 1 performs simple dynamic allocation.

Future versions may introduce

* stack pools
* custom allocators

without changing higher-level modules.

---

# Platform

The platform subsystem isolates operating-system functionality.

Responsibilities

* virtual memory mapping
* page information

The rest of the runtime never calls operating-system APIs directly.

All platform-specific functionality passes through this module.

---

# Common

The common module provides shared infrastructure.

Responsibilities

* compiler abstractions
* project types
* common macros

The common module owns no runtime state.

---

# Data Structures

Generic data structures remain independent from the runtime.

Version 1 includes

* queue
* list
* bitmap
* id\_pool

These structures contain no scheduler-specific or task-specific behavior.

They are reusable outside the runtime.

---

# Ownership Model

Runtime owns

* scheduler
* runtime state
* current task
* main execution context
* task registry
* zombie registry
* task identifier pool

Scheduler owns

* runnable queue

Task owns

* execution context
* stack

Memory manages

* stack allocation
* stack release

Platform owns

* operating-system interaction

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

Execution context is initialized.

↓

Task enters the runtime registry.

↓

Task enters the scheduler queue.

↓

Runtime starts scheduling.

↓

Scheduler selects the next runnable task.

↓

Context subsystem performs the context switch.

↓

Task executes.

↓

Task yields or exits.

↓

Exited tasks enter the zombie registry.

↓

Runtime reaps completed tasks.

↓

Runtime shuts down.

---

# Context Switching

The runtime initiates every context switch.

The scheduler selects the next runnable task.

The context subsystem performs the low-level register save and restore.

Backend implementation details remain isolated behind the Context subsystem.

---

# Error Handling

Recoverable failures return status codes.

Fatal runtime failures terminate through the panic subsystem.

Subsystems must never silently ignore failures.

---

# Extensibility

The architecture is intentionally prepared for future extensions.

Examples include

* alternative context backends
* worker threads
* work stealing
* timers
* reactor
* asynchronous I/O
* custom stack allocators

These extensions should require adding or replacing modules rather than redesigning the architecture.

---

# Non-Goals

Version 1 does not support

* preemptive scheduling
* multiple schedulers
* worker threads
* synchronization primitives
* asynchronous I/O
* lock-free scheduling
* custom memory allocators

---

# Architectural Stability

Version 1 freezes

* subsystem boundaries
* ownership rules
* dependency direction
* layering
* module responsibilities

Implementation details may evolve.

The architecture should remain stable.

---

**End of Document.**
