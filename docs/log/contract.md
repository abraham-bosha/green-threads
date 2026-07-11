# Module: `log`

## Purpose

The logging subsystem provides lightweight runtime diagnostics for development, debugging, and operational visibility. It offers a unified interface for emitting structured log messages while allowing compile-time elimination of disabled log levels.

The logging subsystem is intended for diagnostics only and must never influence runtime correctness.

---

## Responsibilities

* Provide structured runtime logging.
* Support five logging severity levels.
* Attach source location metadata automatically.
* Associate every log message with a subsystem component.
* Remove disabled log levels at compile time.
* Perform compile-time format-string validation where supported.

---

## Non-Responsibilities

The logging subsystem shall not:

* terminate the process;
* enforce runtime invariants;
* allocate dynamic memory;
* maintain global logging state;
* synchronize concurrent writers;
* write to files or external logging backends;
* provide timestamps, colors, or thread identifiers.

---

## Log Levels

The runtime defines five severity levels.

| Level | Purpose                                         |
| ----- | ----------------------------------------------- |
| TRACE | Detailed execution tracing.                     |
| DEBUG | Developer-oriented diagnostic information.      |
| INFO  | Normal runtime events.                          |
| WARN  | Recoverable abnormal conditions.                |
| ERROR | Recoverable runtime errors requiring attention. |

Fatal failures are handled exclusively by the panic subsystem.

---

## Component Identification

Every translation unit using the logging subsystem shall define:

```c
#define GT_LOG_COMPONENT "component-name"
```

before including `log.h`.

This identifies the subsystem that emitted the message.

---

## Compile-Time Filtering

Logging is disabled unless `GT_LOG_ENABLED` is defined.

When enabled, messages below `GT_LOG_LEVEL` are removed entirely during preprocessing.

Disabled logging must introduce zero runtime overhead.

---

## Log Format

Version 1 emits messages in the following format:

```text
[LEVEL][component] file:line function(): message
```

The exact presentation may evolve in future versions without changing the public API.

---

## Design Rules

* Runtime code shall use the `GT_LOG_*` convenience macros.
* Runtime code shall not call `gt_log()` directly except within the logging implementation itself.
* Every log invocation shall include a format string.
* Logging must never alter application behavior.
* Logging failures shall be ignored whenever practical.

---

## Dependencies

### Standard Library

* `<stdio.h>`
* `<stdarg.h>`

### Project

* `common/compiler.h`
* `common/macros.h`

---

## Invariants

* Logging shall remain stateless.
* Logging shall perform no dynamic memory allocation.
* Logging shall require no initialization or shutdown.
* Every emitted message shall contain a severity level and component tag.
* Compile-time disabled log statements shall not generate executable code.

---

## Performance Guarantees

* Header-only dispatch macros.
* Zero runtime cost for disabled log levels.
* No heap allocation.
* No hidden global state.
* Constant-time formatting overhead aside from the underlying C standard I/O functions.

---

## Future Extensions

The current design intentionally leaves room for future enhancements without changing the public interface, including:

* timestamps;
* thread identifiers;
* colored terminal output;
* configurable output streams;
* file logging;
* custom logging backends;
* structured logging formats.

These capabilities shall remain optional and must preserve backward compatibility.

---

End of Document.
