# Runtime Subsystem

## Purpose

The Runtime subsystem is the central coordinator of the Green Threads runtime.

It owns every task, manages the scheduler, controls execution, and provides the public API used to create and execute cooperative green threads.

The Runtime is the only subsystem responsible for orchestrating task execution. It does not implement scheduling algorithms, context switching, or task management itself; instead, it coordinates the specialized subsystems that perform those responsibilities.

---

## Responsibilities

The Runtime subsystem is responsible for:

- Initializing and shutting down the runtime
- Creating and owning tasks
- Managing the runtime lifecycle
- Starting execution
- Performing cooperative context switches
- Scheduling runnable tasks
- Tracking the currently executing task
- Owning the scheduler instance
- Owning the runtime task collection
- Owning the task identifier pool

The Runtime does **not** perform:

- Context switching implementation
- Scheduler queue implementation
- Task initialization
- Stack allocation
- Heap allocation
- Platform-specific operations

Those responsibilities belong to dedicated subsystems.

---

## Public Interface

```c
gt_status_t gt_init(void);

gt_status_t
gt_create_task(
    gt_task_entry_fn entry,
    void *arg,
    size_t stack_size);

void gt_run(void);

void gt_yield(void);

void gt_destroy(void);
```

---

## Internal Interface

The Runtime implementation is internally divided into four components.

### Runtime

Responsible for:

- Runtime initialization
- Runtime destruction
- Global runtime state
- Runtime singleton

### Task Management

Responsible for:

- Runtime-owned task creation
- Runtime-owned task destruction
- Runtime task list management

### Execution

Responsible for:

- Starting execution
- Cooperative yielding
- Task bootstrap
- Task termination

### Scheduler

Responsible for:

- Selecting the next runnable task
- Returning execution to the main context
- Dispatching runnable tasks

---

## Runtime Lifecycle

The runtime progresses through the following lifecycle.

```text
UNINITIALIZED
        │
        ▼
INITIALIZED
        │
        ▼
RUNNING
        │
        ▼
STOPPED
```

Execution begins when `gt_run()` is called.

Execution ends automatically after the last runnable task exits.

---

## Scheduling Model

Version 1 implements a cooperative scheduler.

A task continues executing until it:

- calls `gt_yield()`, or
- returns from its entry function.

Returning from the task entry function automatically terminates the task.

No preemption is performed.

---

## Task Ownership

Every task created through `gt_create_task()` is owned by the Runtime.

The Runtime is responsible for:

- assigning task identifiers
- storing tasks
- destroying completed tasks
- releasing all associated resources

Applications never destroy tasks directly.

---

## Execution Model

Execution begins in the application's main thread.

When `gt_run()` is invoked:

1. The main execution context is saved.
2. The scheduler selects the first runnable task.
3. Control transfers to that task.

Tasks execute cooperatively until all runnable tasks have completed.

When no runnable tasks remain, execution returns to the saved main context and `gt_run()` returns to the caller.

---

## Dependencies

The Runtime subsystem depends on:

- Platform
- Context
- Scheduler
- Task
- ID Pool
- List
- Heap
- Common infrastructure

The Runtime is the highest-level subsystem within the execution engine.

---

## Design Principles

The Runtime is designed around the following principles:

- Single runtime instance
- Single ownership of all tasks
- Cooperative execution
- Intrusive data structures
- Explicit ownership
- Clear subsystem boundaries
- Minimal public API
- Platform-independent behavior

The Runtime coordinates the execution engine while delegating specialized responsibilities to the lower-level subsystems.

---

End of Document.
