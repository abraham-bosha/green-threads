# Page Module

## Purpose

The Page module provides page-granular operations required by the runtime.

It exposes the system page size and provides page alignment and size calculations independent of any virtual memory operations.

The module discovers the platform page size during initialization and caches it for the lifetime of the process.

---

## Responsibilities

The Page module shall:

- Discover the system page size.
- Cache the discovered page size.
- Expose the system page size to the runtime.
- Validate page-aligned addresses.
- Validate page-aligned sizes.
- Round arbitrary sizes up to page boundaries.
- Hide platform-specific page size discovery.

---

## Non-Responsibilities

The Page module shall not:

- Reserve virtual memory.
- Release virtual memory.
- Change virtual memory protection.
- Allocate runtime memory.
- Manage stacks.
- Expose operating-system interfaces.

Those responsibilities belong to the VM or Memory subsystems.

---

## Design Principles

### Runtime-Centric

The module exposes the page concepts required by the runtime rather than the complete capabilities of the underlying operating system.

### Single Source of Truth

The system page size is discovered once and reused throughout the runtime.

No subsystem shall assume a fixed page size.

### Deterministic

After successful initialization, every query returns deterministic results.

The page size never changes during process execution.

### Platform Isolation

Operating-system specific page discovery is hidden behind the platform implementation.

Consumers never invoke Linux or POSIX interfaces directly.

---

## Module Lifetime

The module is initialized during platform initialization.

After successful initialization, the page size remains constant until process termination.

---

## Relationships

The Page module may depend on:

- Platform
- Common

The following subsystems may depend on the Page module:

- VM
- Memory

---

## Error Handling

Initialization failures are reported using runtime status codes.

Query and calculation operations do not report runtime errors.

Programming errors are detected through assertions.

---

## Thread Safety

After initialization, all operations are read-only.

The module contains no mutable runtime state except the cached page size established during initialization.

---

## Version 1 Scope

Version 1 provides:

- Page size discovery
- Page size query
- Address alignment validation
- Size alignment validation
- Safe page-size rounding

Future versions may introduce additional page utilities as required by the runtime.

---

End of Document
