# Assertion Service Contract

## Purpose

The Assertion service validates internal runtime invariants.

Assertions detect programming errors and invalid runtime states that should never occur during correct execution. When an assertion fails, control is transferred to the Panic service for diagnostic reporting and process termination.

---

## Responsibilities

The Assertion service shall:

- Validate internal runtime invariants.
- Capture assertion source location information.
- Capture the failed assertion expression.
- Optionally capture an implementation-defined diagnostic message.
- Forward assertion failures to the Panic service.

---

## Non-Responsibilities

The Assertion service shall not:

- Handle recoverable runtime errors.
- Replace explicit status code returns.
- Perform logging.
- Allocate memory.
- Recover from assertion failures.
- Terminate execution directly.

Process termination is the responsibility of the Panic service.

---

## Internal Interface

The Assertion service is an internal runtime facility.

It provides the following interface:

```c
GT_ASSERT(condition);

GT_ASSERT_MSG(condition, message);
```

No public API is exposed.

---

## Dependencies

### Project Dependencies

- common/compiler
- panic

### Standard Library Dependencies

None.

---

## Dependents

The Assertion service may be used by:

- Runtime
- Scheduler
- Task
- Context
- Memory
- Platform
- Data Structures
- System Services

---

## Error Handling

Assertions represent violated runtime invariants.

A failed assertion is considered a programming error rather than a recoverable runtime error.

Failed assertions shall never return a status code.

Instead, they shall invoke the Panic service.

---

## Invariants

The Assertion service shall satisfy the following invariants:

- A failed assertion shall never return.
- A successful assertion shall have no observable side effects.
- Assertions shall not modify program state.
- Assertions shall not allocate memory.
- Assertions shall not introduce circular dependencies with the Panic service.

---

## Debug and Release Behavior

Debug builds shall enable assertion validation.

Release builds may disable non-critical assertions when explicitly configured.

Critical runtime safety assertions may remain enabled in release builds.

---

## Performance Guarantees

Successful assertions shall introduce minimal runtime overhead.

The Assertion service shall:

- Avoid dynamic memory allocation.
- Avoid global mutable state.
- Be implemented primarily through compile-time macros.

---

## Future Evolution

Future versions may introduce:

- Assertion categories.
- Runtime-configurable assertion levels.
- Expensive debug-only invariant validation.
- Structured diagnostic metadata.

---

## Related Documents

- `docs/panic/contract.md`
- `design/invariants.md`
- `design/project_contract.md`

---

End of Document.
