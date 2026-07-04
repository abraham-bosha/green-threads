# Green Threads Runtime Engine

A lightweight user-space cooperative green threads runtime written in C23, designed for exploring how scheduling, context switching, and runtime systems work at a low level.

This project is not a framework or application. It is a systems-level runtime experiment focused on building and understanding:

- Cooperative multitasking in user space
- Scheduling and execution control
- Minimal runtime abstractions
- Deterministic execution models

---

## Scope

This project focuses strictly on runtime systems concepts:

- User-space thread scheduling (green threads)
- Task lifecycle management
- Context switching abstractions (future backend experiments)
- Memory and execution control at the runtime level
- Performance and correctness trade-offs in concurrency models

### Non-Goals

The project explicitly avoids:

- OS-level kernel thread implementation
- High-level application frameworks
- Blocking or OS-dependent concurrency abstractions as core design elements

---

## Status

**Early-stage systems project (v1)**

Core architecture is under active design and incremental implementation.

---

## Architecture Overview

The runtime is designed as a layered systems library with strict separation between core runtime logic and external consumers.

### Repository Layout

```text
src/            Core runtime implementation
include/        Public API headers
internal/       Internal/private headers

tests/          Unit, integration, stress, and regression tests
examples/       Runnable demonstration programs
benchmarks/     Performance measurement workloads

design/         Architecture decisions and system invariants
docs/           Project documentation

debug/          Debugging utilities and tools
build/          Generated artifacts (not tracked)
```

---

## Core Runtime Model

The system follows a library-first architecture.

### 1. Core Library

The runtime is compiled into a static library:

```text
libgt.a
```

The library contains:

- Scheduler implementation
- Task management primitives
- Runtime execution logic
- Internal synchronization mechanisms

### 2. Runtime Consumers

All execution layers depend on the core library:

- Tests — correctness validation
- Examples — usage demonstrations
- Benchmarks — performance evaluation

Each consumer is built as an independent executable linked against `libgt.a`.

---

## Build System Model

The project uses a Makefile-driven dependency graph build system.

### Build Artifacts

All compiled output is isolated under:

```text
build/<profile>/
```

Supported profiles:

- `debug`
- `release`

Directory structure:

```text
build/<profile>/
        ├── obj/    Object files (.o)
        ├── dep/    Dependency files (.d)
        ├── lib/    Static libraries
        └── bin/    Executables
```

---

## Dependency Structure

The build system follows a strict dependency graph:

```text
        src/*.c
           ↓
        object files (.o)
           ↓
        libgt.a
           ↓
tests / examples / benchmarks
```

### Properties

- Incremental compilation
- Automatic dependency tracking via `.d` files
- Single runtime library artifact
- Thin executable wrappers around the runtime

---

## Design Principles

### Separation of Concerns

Runtime implementation remains isolated from all execution layers.

### Library-Centric Architecture

Everything depends on `libgt.a`.

### Deterministic Build Graph

Dependencies are tracked explicitly through Make.

### Scalable Structure

New modules can be added without changing the build model.

---

## Build System

The Makefile acts as the single source of truth for compilation, linking, and dependency tracking.

### Build Everything

```bash
make all
```

Builds:

- Static runtime library (`libgt.a`)
- Tests
- Examples
- Benchmarks

### Build Only the Runtime Library

```bash
make lib
```

Produces:

```text
libgt.a
```

### Build Specific Targets

```bash
make tests
make examples
make benchmarks
```

Each target produces independent executables linked against the runtime library.

---

## Execution Model

Each subsystem produces standalone executables.

### Run Tests

```bash
make run-tests
```

Behavior:

- Executes all test binaries
- Fails fast on first error
- Used for correctness validation

### Run Examples

```bash
make run-examples
```

Behavior:

- Runs all example programs
- Intended for exploration and demonstration
- Failures do not stop execution

### Run Benchmarks

```bash
make run-benchmarks
```

Behavior:

- Runs performance workloads
- Measures runtime behavior and scalability
- Failures do not stop execution

---

## Development Workflow

### Full Validation Pipeline

```bash
make check
```

Runs:

- Static analysis (`cppcheck`)
- Tests

Used as the primary correctness gate.

### Code Formatting

```bash
make format
```

Formats all source files using `clang-format`.

### Environment Validation

```bash
make check-env
```

Verifies required tooling:

- Clang
- GNU Make
- ar
- cppcheck
- clang-format

---

## Build Artifact Model

All generated output is stored under:

```text
build/<profile>/
```

Where `<profile>` is either:

```text
debug
release
```

Directory layout:

```text
obj/    Compiled object files
dep/    Dependency tracking files
lib/    Static libraries
bin/    Executable binaries
```

Benefits:

- Reproducible builds
- No source tree pollution
- Clear separation of generated artifacts

---

## Quality Model

The project follows a simple development loop:

1. Build

```bash
make all
```

2. Verify Correctness

```bash
make run-tests
```

3. Analyze

```bash
make lint
```

4. Format

```bash
make format
```

Goals:

- Correctness
- Consistency
- Maintainability

---

## Developer Mental Model

When working in this project, think in terms of four layers.

### 1. Runtime Core

Everything originates from `src/` and ultimately becomes:

```text
libgt.a
```

### 2. Consumers

Tests, examples, and benchmarks are clients of the runtime, not part of the runtime itself.

### 3. Build Graph

Make resolves:

```text
    source → objects → libgt.a → executables
```

### 4. Execution Separation

```text
tests       → correctness
examples    → usability
benchmarks  → performance
```

---

## License

Released under the MIT License.
