# Dependency Graph

Version: 1.0

Status: Frozen

---

# Purpose

This document defines the dependency hierarchy of the Green Threads Runtime Engine.

Every module dependency within the runtime must conform to the rules described here.

The dependency graph is considered part of the architecture. Any implementation that violates this document is architecturally incorrect.

---

# Design Goals

The dependency graph exists to guarantee:

- modularity
- maintainability
- replaceability
- testability
- scalability
- predictable architecture
- strict separation of concerns

The runtime is intentionally organized as a Directed Acyclic Graph (DAG). Circular dependencies are forbidden.

---

# Notation

Throughout this document:

- **ds** = Data Structures
- **Platform** = Operating-system abstraction layer
- **Common** = Shared utilities available to all subsystems

---

# Architectural Layers

The runtime is divided into architectural layers.

Higher layers may depend only on lower layers.

Lower layers must never depend on higher layers.

The dependency direction always points downward.

```text

                                     +----------------------------+
                                     |  Public API (include/gt/)  |
                                     +-------------+--------------+
                                                   |
                                                   v
                                         +----------------------+
                                         |       Runtime        |
                                         +----------+-----------+
                                                    |
         +----------------------+-------------------+--------------------+---------------+------------+
         |     |                |                   |                    |               |            |
         v     |                v                   v                    v               v            |     
+-----------+  |           +-----------+      +-----------+         +-----------+    +-----------+    |
| Scheduler |  |           |   Task    |      |  Context  |         |  Memory   |    | Platform  |    |
+------+----+  |           +------+----+      +------+----+         +------+----+    +-----+-----+    |
       |       |                  |                  |                     |               |          |
  +----+----+  |  +---------+-----+----+--------+    +------+--------+-----+               |          |
  |    |    |  |  |         |          |        |           |        |                     v          |
  v    |    v  v  v         v          v        |           v        |         +--------------------+ |    
+----+ |  +---------+   +--------+  +-------+   |       +--------+   |         | Linux              | |
|Task| |  |   Ds    |   | Memory |  |Context|   |       |platform|   |         | POSIX              | |
+----+ |  +---+-----+   +--------+  +-------+   |       +--------+   |         | C Standard Library | |
       |      |                                 |                    |         +--------------------+ |   
       |      |                                 |                    |                                |  
       +------+---------------------------------+---------+----------+--------------------------------+   
                                                          |
                                                          v
                                                     +----------+
                                                     |  Common  |
                                                     +----+-----+
                                                          |
                                                          v
                                                 +--------------------+
                                                 | C Standard Library |
                                                 +--------------------+

```

---

# Architectural Principles

The dependency graph is based on the following principles.

## Library-first Architecture

The runtime is built as a library.

Executable programs (tests, examples, benchmarks) are clients of the runtime rather than part of it.

---

## Stable Foundations

Lower layers should change less frequently than higher layers.

Modules such as Common and ds are intended to be reusable infrastructure.

---

## Platform Isolation

Operating-system functionality is isolated behind the Platform subsystem.

No runtime subsystem may invoke Linux or POSIX interfaces directly.

Every operating-system interaction must pass through Platform.

---

## Data Structure Independence

Data structures are reusable utilities.

They are independent of the runtime implementation and may never depend on runtime subsystems.

They provide generic functionality that can be reused by any subsystem.

# Module Dependencies

This section defines the dependency contract for every subsystem.

For each module:

- **May depend on** defines the only permitted dependencies.
- **Must never depend on** defines permanently forbidden dependencies.

These rules are normative.

---

## Public API

The Public API exposes the runtime interface to users.

It must remain a thin abstraction layer and must never expose implementation details.

### May depend on

- Runtime

### Must never depend on

- Scheduler
- Task
- Context
- Memory
- Platform
- Common
- ds

---

## Runtime

The Runtime coordinates the entire system.

It orchestrates scheduling, task management, context switching, and memory management without containing platform-specific implementation details.

### May depend on

- Scheduler
- Task
- Context
- Memory
- Platform
- Common
- ds

### Must never depend directly on

- Linux
- POSIX
- C Standard Library

All operating-system interaction must occur exclusively through the Platform interface.

```text
Runtime
    │
    ▼
Platform
    │
    ▼
Linux / POSIX
```

---

## Scheduler

The Scheduler determines which task executes next.

It manages runnable tasks but must remain independent of runtime orchestration and operating-system implementation.

### May depend on

- Task
- ds
- Common

### Must never depend on

- Runtime
- Platform
- Context implementation
- Memory implementation

---

## Task

The Task subsystem manages task objects and lifecycle.

It is responsible for representing execution state without making scheduling decisions.

### May depend on

- Memory
- Context
- ds
- Common

### Must never depend on

- Runtime
- Scheduler

---

## Context

The Context subsystem manages execution contexts and context switching.

Platform-specific context implementations are hidden behind this interface.

### May depend on

- Platform
- Common

### Must never depend on

- Runtime
- Scheduler
- Task
- Memory
- ds

---

## Memory

The Memory subsystem manages runtime stack allocation and related facilities.

Memory management is independent of scheduling and task execution.

### May depend on

- Platform
- Common

### Must never depend on

- Runtime
- Scheduler
- Task
- Context
- ds

---

## Platform

The Platform subsystem isolates all operating-system interaction.

Every system call, platform-specific primitive, and OS abstraction belongs here.

### May depend on

- Common
- Linux
- POSIX
- C Standard Library

### Must never depend on

- Runtime
- Scheduler
- Task
- Context
- Memory
- ds

---

## Common

Common provides shared utilities used throughout the runtime.

Examples include:

- assertions
- panic handling
- compiler abstractions
- bit utilities
- macros
- utility helpers

### May depend on

- C Standard Library

### Must never depend on

- Runtime
- Scheduler
- Task
- Context
- Memory
- Platform
- ds

---

## ds (Data Structures)

The ds subsystem contains reusable, implementation-independent data structures.

Data structures are generic infrastructure and must remain completely independent of runtime logic.

### May depend on

- Common

### Must never depend on

- Runtime
- Scheduler
- Task
- Context
- Memory
- Platform

---

# Data Structure Dependencies

Version 1 includes the following data structures.

## queue

Depends on

- Common

---

## bitmap

Depends on

- Common

---

## id\_pool

Depends on

- bitmap
- Common

---

# Subsystem Dependency Hierarchy

The following diagrams summarize the allowed dependencies for each subsystem.

## Runtime

```text
Runtime
│
├── Scheduler
├── Task
├── Context
├── Memory
├── Platform
└── Common
```

---

## Scheduler

```text
Scheduler
│
├── Task
├── ds
└── Common
```

---

## Task

```text
Task
│
├── Context
├── Memory
├── ds
└── Common
```

---

## Context

```text
Context
│
├── Platform
└── Common
```

---

## Memory

```text
Memory
│
├── Platform
└── Common
```

---

## Platform

```text
Platform
│
├── Common
├── Linux
├── POSIX
└── C Standard Library
```

---

## ds

```text
ds
│
├── queue
├── bitmap
└── id_pool
```

---

# Allowed Dependency Graph

The following graph summarizes every dependency permitted by Version 1.

```text
Public API
│
└── Runtime
    │
    ├── Scheduler
    ├── Task
    ├── Context
    ├── Memory
    ├── Platform
    └── Common

Scheduler
│
├── Task
├── ds
└── Common

Task
│
├── Context
├── Memory
├── ds
└── Common

Context
│
├── Platform
└── Common

Memory
│
├── Platform
└── Common

Platform
│
├── Common
├── Linux
├── POSIX
└── C Standard Library

ds
│
├── Common
├── queue
├── bitmap
└── id_pool

Common
│
└── C Standard Library
```

This graph is exhaustive.

Any dependency not represented here is forbidden unless explicitly approved through an architectural revision.

---

# Forbidden Dependencies

The following dependency directions are permanently prohibited.

## Runtime

```text
Runtime → Linux
Runtime → POSIX
Runtime → C Standard Library
```

---

## Scheduler

```text
Scheduler → Runtime
Scheduler → Platform
Scheduler → Context
Scheduler → Memory
```

---

## Task

```text
Task → Runtime
Task → Scheduler
```

---

## Context

```text
Context → Runtime
Context → Scheduler
Context → Task
Context → Memory
Context → ds
```

---

## Memory

```text
Memory → Runtime
Memory → Scheduler
Memory → Task
Memory → Context
Memory → ds
```

---

## Platform

```text
Platform → Runtime
Platform → Scheduler
Platform → Task
Platform → Context
Platform → Memory
Platform → ds
```

---

## Common

```text
Common → Runtime
Common → Scheduler
Common → Task
Common → Context
Common → Memory
Common → Platform
Common → ds
```

---

## ds

```text
ds → Runtime
ds → Scheduler
ds → Task
ds → Context
ds → Memory
ds → Platform
```

---

# Circular Dependencies

Circular dependencies are strictly prohibited.

The dependency graph must always remain a **Directed Acyclic Graph (DAG).**

Examples of invalid designs include:

```text
Scheduler
     │
     ▼
Task
     ▲
     │
Context
```

---

```text
Task
     │
     ▼
Context
     ▲
     │
Memory
```

---

```text
Memory
     │
     ▼
Platform
     ▲
     │
Common
```

Introducing any dependency that forms a cycle is considered an architectural defect.

---

# Dependency Inversion

Whenever practical, modules should communicate through interfaces rather than concrete implementations.

Examples:

- Runtime communicates with Context through the context interface.
- Runtime communicates with Memory through the memory interface.
- Runtime communicates with Platform through the platform interface.
- Scheduler remains independent of the active context-switch backend.
- Task remains independent of the underlying stack allocator.

This allows implementations to evolve without changing higher-level modules.

---

# Future Extensions

Future modules must preserve the dependency hierarchy defined in this document.

New functionality should be introduced as additional leaves or intermediate layers without violating existing rules.

Examples include:

```text
Worker
    │
    ▼
Scheduler
```

---

```text
Reactor
    │
    ▼
Scheduler
```

---

```text
Timer
    │
    ▼
Scheduler
```

---

```text
Assembly Backend
        │
        ▼
Context
```

---

```text
Profiler
     │
     ▼
Runtime
```

---

```text
Tracing
     │
     ▼
Runtime
```

Every proposed subsystem must define:

- its permitted dependencies
- its forbidden dependencies
- its architectural layer
- its public interface

before implementation begins.

---

# Architectural Review

Any dependency that violates this document requires an architectural review before implementation.

The review must determine:

- whether the dependency is truly necessary
- whether an interface can eliminate the dependency
- whether the dependency introduces coupling
- whether the dependency preserves the DAG

Architectural convenience is never sufficient justification for violating the dependency hierarchy.

---

# Stability

This dependency graph is frozen for **Version 1**.

The following properties are considered stable:

- subsystem boundaries
- dependency directions
- architectural layers
- platform isolation
- Common subsystem responsibilities
- ds independence

Future versions may extend the graph but must preserve these foundational principles unless an explicit architectural revision supersedes this document.

---

End of Document.
