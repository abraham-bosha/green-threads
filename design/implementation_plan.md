# Implementation Plan

Version: 1.0

Status: Draft

---

# Purpose

This document defines the implementation order of Version 1.

The order is dependency-driven rather than directory-driven.

Each stage should produce a working, testable system before moving to the next stage.

No stage should begin until the previous stage is complete.

---

# Development Philosophy

Version 1 is built incrementally.

Every completed stage must satisfy three conditions.

- builds successfully
- passes its tests
- leaves the repository in a working state

The runtime should never remain broken between stages.

---

# Stage 0

Repository Preparation

## Goal

Prepare the development environment.

## Tasks

- repository structure
- Makefile
- compiler flags
- formatting
- linting
- testing framework
- example build system

## Deliverable

The project builds successfully even though no runtime exists.

---

# Stage 1

Common Infrastructure

## Goal

Create the shared foundation.

## Modules

common/

## Tasks

- project types
- compiler abstraction
- macros
- bit utilities

## Deliverable

Every subsystem can safely depend on Common.

---

# Stage 2

Generic Data Structures

## Goal

Implement reusable data structures.

## Modules

ds/

## Order

1. bitmap
2. id\_pool
3. queue

Each structure must have unit tests before continuing.

## Deliverable

Reusable data structure library.

---

# Stage 3

Platform Layer

## Goal

Isolate operating-system functionality.

## Modules

platform/

## Tasks

- mmap wrapper
- page information

## Deliverable

No runtime code directly calls operating-system APIs.

---

# Stage 4

Memory

## Goal

Allocate and release task stacks.

## Modules

memory/

## Tasks

- stack allocation
- stack destruction

Version 1 intentionally excludes

- pools
- guard pages
- statistics

## Deliverable

Stack allocation works independently.

---

# Stage 5

Context

## Goal

Create execution contexts.

## Modules

context/

## Backend

ucontext

## Tasks

- create context
- destroy context
- switch context

The backend must remain isolated.

## Deliverable

Two contexts can switch execution successfully.

---

# Stage 6

Task

## Goal

Represent executable work.

## Modules

task/

## Tasks

- task creation
- task destruction
- lifecycle
- task state
- ID allocation

Every task owns

- stack
- context
- identifier

## Deliverable

Tasks can exist independently of scheduling.

---

# Stage 7

Scheduler

## Goal

Execute runnable tasks.

## Modules

scheduler/

## Scheduling Policy

FIFO

## Tasks

- enqueue
- dequeue
- dispatch
- yield

The scheduler does not perform context switching.

It requests the Context subsystem to perform the switch.

## Deliverable

Tasks execute cooperatively.

---

# Stage 8

Runtime

## Goal

Connect all subsystems.

## Modules

runtime/

## Tasks

- initialization
- shutdown
- configuration
- public API

The runtime owns every subsystem.

## Deliverable

A complete cooperative runtime.

---

# Stage 9

Examples

## Goal

Validate usability.

## Examples

- hello world
- yielding
- multiple tasks
- producer consumer

Examples should remain small and educational.

---

# Stage 10

Testing

## Goal

Verify correctness.

## Levels

Unit Tests

- every module

Integration Tests

- runtime
- scheduler
- task
- context

Stress Tests

- large task counts
- repeated context switches
- stack allocation

Regression Tests

- bug fixes

Every discovered bug should receive a regression test.

---

# Stage 11

Benchmarking

## Goal

Measure runtime performance.

## Benchmarks

- task creation
- context switching
- scheduler throughput
- stack allocation
- data structures

Benchmarking begins only after correctness.

---

# Completion Criteria

A stage is complete only when

- implementation compiles
- tests pass
- documentation is updated
- examples still execute
- benchmarks remain valid

No incomplete stage may be skipped.

---

# Possible Future Versions

                     Runtime Evolution

v1  Cooperative Runtime
        │
        ▼
v2  Multiple Context Backends
        │
        ▼
v3  Timers
        │
        ▼
v4  Reactor
        │
        ▼
v5  Worker Pool
        │
        ▼
v6  Work-Stealing
        │
        ▼
v7  Synchronization
        │
        ▼
v8  Preemptive Runtime
        │
        ▼
v9  Production Runtime


                Context Backend Evolution

           ucontext
               │
               ▼
           setjmp
               │
               ▼
           assembly

These versions are outside the Version 1 implementation plan.

---

# Implementation Rules

Always implement in dependency order.

Never implement a module whose dependencies are incomplete.

Never bypass architectural boundaries.

Never sacrifice correctness for speed of implementation.

Prefer simple implementations over clever implementations.

Every feature must have a test before the stage is considered complete.

---

End of Document.
