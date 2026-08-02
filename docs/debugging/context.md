# Context Switching Bug: Wrapper Functions Around Assembly

## Overview

During the initial implementation of the x86-64 context switching backend, the
runtime repeatedly crashed after switching back from a task to the main thread
or another task.

The crash consistently occurred immediately after restoring a previously saved
execution context.

The root cause was **not** in the assembly context-switch implementation itself.
Instead, it resulted from introducing ordinary C wrapper functions around the
low-level assembly primitives.

---

# Original Design

The runtime originally used the following call chain.

```text
gt_runtime_run()
    └── gt_context_save()
            └── __gt_context_save_asm()
```

and

```text
__gt_runtime_resume_main()
    └── gt_context_load()
            └── __gt_context_load_asm()
```
and 


```text
__gt_runtime_resume_task()
    └── gt_context_load()
            └── __gt_context_load_asm()
```

where

- `gt_context_save()` was an ordinary C function
- `gt_context_load()` was an ordinary C function
- both simply delegated to the assembly routines

---

# Why This Looked Correct

The wrappers performed no additional work.

```c
int
gt_context_save(struct gt_context *ctx)
{
    return __gt_context_save_asm(ctx);
}

GT_NORETURN void
gt_context_load(const struct gt_context *ctx)
{
    __gt_context_load_asm(ctx);
}
```

From a normal C perspective this appears harmless.

Unfortunately, a context switch is **not** a normal function call.

---

# What Actually Happens

Assume execution begins here.

```text
gt_runtime_run()
{
    if (gt_context_save(&main_ctx) == 0)
    {
        scheduler();
    }
}
```

The processor performs the following sequence.

---

## Step 1

`gt_runtime_run()` calls `gt_context_save()`.

The CPU pushes the return address.

```text
Stack

+----------------------+
| return -> runtime    |
+----------------------+
```

---

## Step 2

`gt_context_save()` calls `__gt_context_save_asm()`.

The CPU pushes another return address.

```text
Stack

+------------------------------+
| return -> gt_context_save     |
+------------------------------+
| return -> gt_runtime_run      |
+------------------------------+
```

---

## Step 3

`__gt_context_save_asm()` stores the current value of `RSP`.

This saved stack pointer now points at

```text
return -> gt_context_save
```

not

```text
return -> gt_runtime_run
```

This distinction is critical.

---

## Step 4

`__gt_context_save_asm()` returns.

Execution continues inside

```text
gt_context_save()
```

which immediately returns to

```text
gt_runtime_run()
```

The wrapper frame disappears.

Execution continues normally.

Eventually the scheduler switches to another task.

---

## Step 5

Much later the runtime restores the saved main context.

```text
gt_context_load()
        ↓
__gt_context_load_asm()
```

The assembly restores

```
RSP
RBX
RBP
R12
R13
R14
R15
```

and finally executes

```asm
mov $1, %eax
ret
```

---

# The Problem

The restored stack pointer still contains the return address that originally
returned into

```
gt_context_save()
```

However, that wrapper no longer exists as an active call frame.

It already returned long before the the runtime scheduler executed.

The restored continuation therefore attempts to resume execution inside a call
frame that no longer exists.

Execution immediately becomes undefined.

Depending on the compiler and optimization level this produced

- invalid return addresses
- corrupted stack state
- segmentation faults
- impossible backtraces

---

## Resolution

The wrapper functions were converted into forced inline functions.

```c
GT_FORCE_INLINE int
gt_context_save(struct gt_context *current)
{
    return __gt_context_save_asm(current);
}

GT_FORCE_INLINE GT_NORETURN void
gt_context_load(const struct gt_context *next)
{
    __gt_context_load_asm(next);
}
```

With forced inlining, no wrapper stack frame exists.

The generated code becomes effectively:

```text
gt_runtime_run()
        │
        ▼
__gt_context_save_asm()
```

and

```text
__gt_context_load_asm()
        │
        ▼
ret
        │
        ▼
gt_runtime_run()
```

The saved return address now points directly to the original caller, preserving
a perfectly symmetric save/restore sequence.

## Lesson Learned

Context-switch routines restore an entire execution stack exactly as it existed
when it was saved. Any additional non-inlined wrapper frame becomes part of that
saved state.

For low-level context-switch primitives, wrapper functions must either:

- be forced to inline completely, or
- be avoided entirely.

Otherwise, the restored execution stack may no longer correspond to the actual
execution flow, leading to undefined behavior.
