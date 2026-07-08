# Panic Service Contract

The **Panic** service provides the Green Threads runtime's fatal error handling mechanism.

A panic represents an unrecoverable internal failure caused by a violated runtime invariant. Once a panic is triggered, the runtime immediately emits diagnostic information and terminates the process. Execution never resumes.

---

# Purpose

Provide a single, centralized mechanism for reporting fatal runtime failures and safely terminating the process.

---

# Responsibilities

- Report unrecoverable runtime failures.
- Print diagnostic information.
- Identify the failure location.
- Terminate the process.
- Guarantee that execution never resumes.

---

# Non-Responsibilities

The Panic service does **not**:

- Handle recoverable errors.
- Perform logging.
- Allocate memory.
- Attempt recovery.
- Continue execution.

---

# Public Interface

None.

The Panic service is an internal runtime component.

---

# Internal Interface

```c
GT_NORETURN
void gt_panic(
    const char *file,
    int line,
    const char *function,
    const char *message);
```

---

# Dependencies

## Standard Library

- `<stdio.h>`
- `<stdlib.h>`

## Project

- `compiler.h`

---

# Dependents

The Panic service may be used by any runtime subsystem, including:

- Runtime
- Scheduler
- Task
- Context
- Memory
- Platform
- Data Structures
- Assertions

---

# Error Handling

The Panic service does not return error codes.

Every invocation terminates the process.

---

# Invariants

- Every panic must terminate execution.
- Panic must never return.
- Diagnostic output must be emitted before termination whenever possible.
- Panic must not allocate dynamic memory.
- Panic must not depend on higher-level runtime services.

---

# Preconditions

- The caller has detected an unrecoverable internal failure.
- Continuing execution would violate runtime correctness.

---

# Postconditions

- Diagnostic information has been written to the standard error stream.
- The process has been terminated.

---

# Performance Guarantees

- Constant-time execution.
- Zero heap allocation.
- Minimal dependency footprint.

---

# Future Extensions

Future versions may support:

- Stack backtraces
- Colored diagnostics
- Signal integration
- Crash reports
- Core dump metadata
- Custom panic handlers
- Debugger integration

---

# Related Documents

- `design/architecture.md`
- `design/invariants.md`
- `docs/assert/contract.md`
