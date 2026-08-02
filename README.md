# Green Threads Runtime Engine

A lightweight user-space cooperative green threads runtime written in **C23**.

The project is built as a systems programming exercise to explore how runtime systems work internally, including scheduling, context switching, task management, and execution control.

---

## Features

Current release (**v1.0.0**) provides:

- Cooperative user-space scheduling
- FIFO task scheduler
- x86-64 assembly context switching backend
- Independent task stacks
- Configurable stack sizes
- Nested task creation
- Runtime reinitialization
- Static runtime library (`libgt.a`)
- Unit, integration, and stress tests
- Runnable example programs

---

## Project Scope

The runtime focuses on low-level runtime systems concepts:

- Green threads
- Cooperative multitasking
- Task lifecycle management
- Context switching
- Runtime execution
- Memory management
- Runtime architecture

### Non-Goals

Version 1 intentionally excludes:

- Preemptive scheduling
- Kernel threads
- Synchronization primitives
- Asynchronous I/O
- Worker thread pools
- Work stealing

---

## Quick Start

Clone the repository:

```bash
git clone https://github.com/abraham-bosha/green-threads.git
cd green-threads
```

Build everything:

```bash
make all
```

Run the test suite:

```bash
make run-tests
```

Run the examples:

```bash
make run-examples
```

Run the benchmarks:

```bash
make run-benchmarks
```

---

## Repository Layout

```text
src/            Runtime implementation
include/        Public API
internal/       Internal implementation

tests/          Unit, integration, and stress tests
examples/       Runnable example programs
benchmarks/     Performance benchmarks

design/         Architecture and design documents
docs/           Module documentation
```

---

## Build System

The runtime is built as a static library:

```text
libgt.a
```

Tests, examples, and benchmarks are compiled as independent executables that link against the runtime library.

Build artifacts are generated under:

```text
build/
```

---

## Example Programs

The repository includes progressively more advanced examples:

| Example |          Description                 |
|---------|--------------------------------------|
| 01      | Runtime initialization               |
| 02      | Single task                          |
| 03      | Multiple tasks                       |
| 04      | Cooperative yielding                 |
| 05      | Task arguments                       |
| 06      | Nested task creation                 |
| 07      | Custom stack sizes                   |
| 08      | Runtime reinitialization             |
| 09      | Fibonacci computation                |
| 10      | Producer–processor–consumer pipeline |
| 11      | Version information                  |

---

## Documentation

Additional documentation is organized into:

- **design/** — architecture, dependency graph, implementation plan, coding standards, and project invariants.
- **docs/** — subsystem contracts, runtime documentation, and debugging notes.

---

## License

Released under the MIT License.
