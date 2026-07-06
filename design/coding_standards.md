# Coding Standards

Version: 1.0

Status: Frozen

---

# Purpose

This document defines the coding standards for the Green Threads Runtime.

The purpose of these standards is to ensure that the codebase remains:

- consistent
- readable
- maintainable
- predictable
- easy to review
- easy to refactor

All source code in this repository shall follow these rules unless an exception is explicitly documented.

---

# Language Standard

The project uses:

- C23

Compiler-specific extensions shall not be used unless explicitly documented.

---

# Compiler Warnings

Warnings are treated as errors.

Minimum warning set:

- `-Wall`
- `-Wextra`
- `-Wpedantic`
- `-Werror`

The repository shall compile without warnings.

---

# Naming Convention

All identifiers belonging to the Green Threads project shall use the `gt_` or `GT_` prefix.

Examples:

Functions

```c
gt_runtime_init();
gt_runtime_run();
gt_scheduler_enqueue();
```

Types

```c
gt_runtime_t;
gt_task_t;
gt_status_t;
```

Structures

```c
struct gt_runtime;
struct gt_task;
```

Enumerations

```c
enum gt_task_state;
```

Enumeration Values

```c
GT_TASK_READY
GT_TASK_RUNNING
GT_TASK_FINISHED
```

Macros

```c
GT_ASSERT(...)
GT_PAGE_SIZE
GT_DEFAULT_STACK_SIZE
```

---

# File Naming

Source files use:

- snake\_case

Examples:

```
runtime.c
task_manager.c
context_switch.c
stack_allocator.c
```

Do not use:

- camelCase
- PascalCase
- mixedCase

---

# Directory Naming

Directories use:

- snake\_case

Examples:

```
runtime/
scheduler/
memory/
context/
platform/
```

---

# Variables

Variables use:

- snake\_case

Examples:

```c
current_task
stack_size
task_count
```

Single-letter variable names are permitted only for simple loop variables.

---

# Functions

Function names use:

```
subsystem_action()
```

Examples:

```c
gt_runtime_init()
gt_runtime_shutdown()
gt_task_create()
gt_context_switch()
```

Avoid generic names such as:

```
create()
destroy()
run()
execute()
```

---

# Macros

Macros use:

- UPPER\_SNAKE\_CASE

Examples:

```c
GT_ASSERT(...)
GT_MIN(...)
GT_MAX(...)
GT_PAGE_SIZE
```

---

# Enumerations

Enumeration names use:

```c
enum gt_task_state;
```

Enumeration values use:

```c
GT_TASK_READY
GT_TASK_RUNNING
GT_TASK_WAITING
GT_TASK_FINISHED
```

---

# Structures

Structures shall not be typedef'd.

Public interfaces expose opaque forward declarations.

Example:

```c
struct gt_runtime;
```

Internal implementation provides the definition.

Example:

```c
struct gt_runtime
{
    ...
};
```

Code shall refer to structures using the struct keyword.

---

# Function Declarations

Function declarations and definitions shall place the return type on a separate
line from the function name.

This convention applies uniformly to:

- public functions
- internal functions
- static functions
- extern declarations
- the `main()` function

Example:

```c
const char *
gt_version_string(void);

static void
scheduler_run(void);

int
main(void);
```

---

## Brace Style

```markdown
# Brace Style

Opening braces shall always appear on a new line.

This rule applies uniformly to all language constructs, including:

- functions
- if statements
- else statements
- switch statements
- loops
- structures
- unions
- enumerations

Examples:

```c
if (condition)
{
    ...
}

struct gt_task
{
    ...
};

enum gt_status
{
    ...
};

int
main(void)
{
    ...
}
```

---

# Header Files

Every header shall begin with:

```c
#pragma once
```

Headers shall include only the dependencies they require.

Public headers shall never include:

```
gt_internal/
```

---

# Include Order

Source files shall include headers in the following order:

1. Module's own header
2. Standard Library
3. Third-party libraries
4. Public headers
5. Internal headers

Example:

```c
#include "scheduler.h"

#include <stddef.h>
#include <stdint.h>

#include <gt/error.h>

#include <gt_internal/queue.h>
#include <gt_internal/context.h>
```

---

# Function Design

Each function should perform one well-defined responsibility.

Avoid large functions.

As a general guideline:

- Prefer functions under approximately 100 logical lines.

Split functionality rather than creating large multi-purpose functions.

---

# Source File Size

Source files should remain focused.

As a guideline:

- Prefer fewer than approximately 500 lines per source file.

Split files by responsibility whenever appropriate.

---

# Error Handling

Public functions return:

```c
gt_status_t
```

Recoverable failures return an appropriate status value.

Programmer mistakes are detected using assertions.

Internal code shall not silently ignore failures.

---

# Ownership

Every dynamically allocated object shall have exactly one owner.

Ownership transfers shall be explicit.

Borrowed pointers shall never be freed.

Ownership rules shall be documented whenever necessary.

---

# Memory Allocation

Memory allocation shall occur only through the memory subsystem.

Direct calls to:

```
malloc()
calloc()
realloc()
free()
```

outside the memory subsystem are prohibited unless explicitly justified.

---

# Comments

Comments explain:

- why

Not:

- what

Good:

```c
/* Reserve task ID zero for the idle task. */
```

Bad:

```c
/* Increment i. */
```

---

# Logging

Runtime code shall not call:

```c
printf()
```

Use the project's logging facilities instead.

---

# Assertions

Assertions verify programmer assumptions.

Assertions shall never replace runtime error handling.

---

# Module Boundaries

Modules communicate only through documented interfaces.

Dependencies shall follow the project's dependency graph.

Circular dependencies are prohibited.

---

# Data Structures

The `ds` library is generic.

It shall never depend upon runtime-specific modules.

Examples:

```
runtime
scheduler
task
memory
platform
```

Generic data structures must remain reusable across future projects.

---

# Formatting

Use consistent indentation throughout the project.

Guidelines:

- One statement per line.
- One declaration per line.
- Keep nesting shallow.
- Prefer early returns when they improve readability.
- Avoid unnecessary whitespace.

Consistency is more important than personal preference.

---

# Documentation

Every public function shall document:

- Purpose
- Parameters
- Return value
- Ownership
- Possible errors

---

# General Principles

Prefer:

- simplicity over cleverness
- readability over brevity
- explicit behavior over implicit behavior
- correctness before optimization

Respect architectural boundaries.

The code should be understandable without requiring knowledge of its implementation history.

---

End of Document.
