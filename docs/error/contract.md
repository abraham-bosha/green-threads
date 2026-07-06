# Error Subsystem Contract

# Purpose

The error subsystem defines the status reporting model of the Green Threads Runtime.

Its responsibility is to communicate the outcome of public runtime operations in a
consistent, predictable, and implementation-independent manner.

The subsystem does not perform error handling itself.

---

# Scope

The error subsystem is responsible for:

- defining the public status type
- defining public status codes
- grouping status codes by subsystem
- maintaining stable numeric assignments
- providing human-readable status descriptions

The subsystem is not responsible for:

- logging
- tracing
- assertions
- panic handling
- debugging
- diagnostics
- exception mechanisms

---

# Design Goals

The status reporting model shall be:

- simple
- deterministic
- lightweight
- implementation-independent
- ABI-stable
- easy to inspect
- easy to extend

---

# Status Model

Every public operation shall report its execution result using a status code.

Successful operations shall return the success status.

Recoverable failures shall return an appropriate subsystem-specific status.

Fatal failures are outside the responsibility of this subsystem.

---

# Status Ownership

Status codes belong to the subsystem that defines the operation.

Each subsystem owns an exclusive numeric range.

Status codes shall never migrate between subsystem ranges.

---

# Numeric Layout

Status codes are grouped by subsystem.

Each subsystem owns a reserved numeric range.

Numeric assignments are permanent once released.

Unused values remain reserved for future expansion.

---

# Generic Status Codes

The runtime defines a small collection of generic status codes.

These represent failures that may occur across multiple subsystems.

Examples include:

- success
- invalid argument
- invalid state
- out of memory
- unsupported operation
- internal error

---

# Subsystem Status Codes

Subsystem-specific failures shall remain within the subsystem's numeric range.

Examples include:

- runtime initialization failures
- scheduler failures
- memory failures
- task failures
- context failures

Future runtime components shall receive their own reserved ranges when introduced.

---

# Error Messages

Every public status code shall have a stable, human-readable string representation.

Applications may use these strings for:

- diagnostics
- logging
- debugging
- user-facing messages

The textual representation is informational only.

Applications shall never depend on the contents of these strings.

---

# Thread Safety

The status subsystem maintains no mutable global state.

Status queries shall be thread-safe.

---

# Stability

Numeric status values are part of the public ABI.

Once released, numeric assignments shall never change.

New status codes may be appended within reserved subsystem ranges without modifying existing assignments.

---

# Future Evolution

Future versions may introduce additional subsystem ranges, including:

- reactor
- timers
- worker pool
- work-stealing scheduler
- preemptive scheduler

The existing numeric layout shall remain stable across future versions.

---

# API Status

The public interface remains under design.

The API will not be frozen until Version 1 reaches feature completion.
