#pragma once

/**
 * @file subprocess.h
 * @brief Utilities for verifying subprocess behavior in unit tests.
 */

/**
 * @brief Executes a function in a child process and verifies that it
 *        terminates by calling abort().
 *
 * @param func Function executed in the child process.
 */
void
verify_process_aborts(void (*func)(void));

/**
 * @brief Executes a callback in a child process and captures its output.
 *
 * Both stdout and stderr are redirected to an internal pipe. The callback
 * executes in the child process and exits normally. The parent collects the
 * emitted output and passes it to the supplied verifier.
 *
 * @param func     Function executed in the child process.
 * @param verify   Callback used to inspect the captured output.
 */
void
verify_process_output(void (*func)(void), void (*verify)(const char *output));
