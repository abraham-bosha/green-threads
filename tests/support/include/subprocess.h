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
