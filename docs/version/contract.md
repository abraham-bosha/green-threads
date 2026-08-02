# Version Subsystem

## Purpose

Provides compile-time version information and lightweight runtime queries for the library version.

---

## Responsibilities

- Define semantic version components at compile time.
- Expose the library version through a small runtime API.
- Perform all version queries without dynamic memory allocation.

---

## Public Interface

### Compile-Time

- `GT_VERSION_MAJOR`
- `GT_VERSION_MINOR`
- `GT_VERSION_PATCH`
- `GT_VERSION_NUMBER`
- `GT_VERSION_STRING`

### Runtime

- `const char *gt_version_string(void)`
- `uint32_t gt_version_number(void)`

---

## Dependencies

None.

## Dependents

- Public applications using the library.
- Build systems and compatibility checks.

---

## Invariants

- Runtime version information is immutable.
- Version queries never modify program state.
- `gt_version_number()` always matches `GT_VERSION_NUMBER`.
- `gt_version_string()` always matches `GT_VERSION_STRING`.

---

## Performance

- Compile-time: Constant-time preprocessing.
- Runtime: Constant-time execution with no heap allocation.

---

End of Document.
