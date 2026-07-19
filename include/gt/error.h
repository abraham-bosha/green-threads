#pragma once

/**
 * Public status reporting interface.
 */

typedef enum
{
    /*
     * ============================================================================
     * Generic Status Codes
     * ============================================================================
     */

    /*
     * Successful operation.
     */
    GT_STATUS_SUCCESS = 0,

    /*
     * Generic failures.
     */
    GT_STATUS_INVALID_ARGUMENT = 1,
    GT_STATUS_INVALID_STATE = 2,
    GT_STATUS_OUT_OF_MEMORY = 3,
    GT_STATUS_UNSUPPORTED = 4,
    GT_STATUS_INTERNAL_ERROR = 5,

    /*
     * =========================================================================
     * Runtime Status Codes (100–199)
     * =========================================================================
     */

    GT_STATUS_RUNTIME_ALREADY_INITIALIZED = 100,
    GT_STATUS_RUNTIME_NOT_INITIALIZED = 101,
    GT_STATUS_RUNTIME_RUNNING = 102,

    /*
     * =========================================================================
     * Task Status Codes (200–299)
     * =========================================================================
     */

    GT_STATUS_TASK_LIMIT_REACHED = 200,
    GT_STATUS_TASK_NOT_FOUND = 201,

    /*
     * =========================================================================
     * Scheduler Status Codes (300–399)
     * =========================================================================
     */

    /*
     * Reserved.
     */

    /*
     * =========================================================================
     * Context Status Codes (400–499)
     * =========================================================================
     */

    /*
     * Reserved.
     */

    /*
     * =========================================================================
     * Memory Status Codes (500–599)
     * =========================================================================
     */

    /*
     * Reserved.
     */

    /*
     * =========================================================================
     * Platform Status Codes (600–699)
     * =========================================================================
     */
    GT_STATUS_PAGE_SIZE_UNAVAILABLE = 603,

    GT_STATUS_VM_RESERVATION_FAILED = 600,
    GT_STATUS_VM_PROTECTION_FAILED = 601,
    GT_STATUS_VM_RELEASE_FAILED = 602,

} gt_status_t;

/**
 * Returns a human-readable description of a status code.
 *
 * The returned string has static storage duration and must not be modified.
 *
 * @param status
 *      Status code.
 *
 * @return
 *      Constant string describing the status.
 */
const char *
gt_status_string(gt_status_t status);
