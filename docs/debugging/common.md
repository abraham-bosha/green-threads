# Engineering Log: Subsystem Debugging & Compiler Diagnostics

This document records important implementation issues encountered during the development of the Green Threads runtime and how they were resolved.

---

## 1. ISO Standard C Compilation vs. GNU Statement Extensions

### Symptom

The `ds/list` module failed to compile with the following error:

```text
error: use of GNU statement expression extension from macro expansion
```

### Cause

The original `GT_CONTAINER_OF` macro used the GNU statement expression syntax:

```c
({
    ...
})
```

This is a GCC/Clang extension and is **not** part of the ISO C standard.

The project is compiled in strict C23 mode, so GNU language extensions are rejected.

### Resolution

`GT_CONTAINER_OF` was rewritten as a standard C expression using pointer arithmetic and `offsetof()`.

```c
#define GT_CONTAINER_OF(ptr, type, member) \
    ((type *)((char *)(ptr) - offsetof(type, member)))
```

### Lesson

Prefer ISO C constructs over compiler-specific extensions unless there is a compelling reason to require them. This keeps the runtime portable and compatible with strict compiler settings.
