# Examples

This directory contains small, self-contained programs demonstrating the public
Green Threads API.

The examples are ordered from basic runtime usage to more advanced cooperative
multitasking patterns.

|       Example               |                  Description                                    |
|-----------------------------|-----------------------------------------------------------------|
| `01_runtime_init.c`         | Initialize and destroy the runtime.                             |
| `02_single_task.c`          | Create and execute a single task.                               |
| `03_multiple_tasks.c`       | Schedule multiple tasks using FIFO execution.                   |
| `04_yield.c`                | Cooperative context switching using `gt_yield()`.               |
| `05_task_arguments.c`       | Pass arguments to tasks.                                        |
| `06_nested_tasks.c`         | Create tasks from within another task.                          |
| `07_stack_sizes.c`          | Specify custom task stack sizes.                                |
| `08_runtime_reinitialize.c` | Reinitialize the runtime across multiple lifecycle generations. |
| `09_fibonacci.c`            | Cooperative computation using multiple tasks.                   |
| `10_pipeline.c`             | Implement a simple producer–processor–consumer pipeline.        |
| `11_version_information.c`  | Query compile-time and runtime version information.             |

## Building

Build all examples:

```bash
make examples
```

Run all examples:

```bash
make run-examples
```

Run a single example:

```bash
./build/debug/bin/examples/03_multiple_tasks
```

## Notes

These programs are intentionally minimal and are intended to demonstrate API
usage rather than production application structure.
