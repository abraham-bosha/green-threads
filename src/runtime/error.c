#include <gt/error.h>

const char *
gt_status_string(gt_status_t status)
{
    switch (status)
    {
            /*
             * =========================================================================
             * Generic Status Codes (0 - 99)
             * =========================================================================
             */

        case GT_STATUS_SUCCESS:
            return "success";

        case GT_STATUS_INVALID_ARGUMENT:
            return "invalid argument";

        case GT_STATUS_INVALID_STATE:
            return "invalid state";

        case GT_STATUS_OUT_OF_MEMORY:
            return "out of memory";

        case GT_STATUS_UNSUPPORTED:
            return "unsupported operation";

        case GT_STATUS_INTERNAL_ERROR:
            return "internal error";

            /*
             * =========================================================================
             * Runtime Status Codes (100–199)
             * =========================================================================
             */

        case GT_STATUS_RUNTIME_ALREADY_INITIALIZED:
            return "runtime already initialized";

        case GT_STATUS_RUNTIME_NOT_INITIALIZED:
            return "runtime not initialized";

        case GT_STATUS_RUNTIME_RUNNING:
            return "runtime already running";

            /*
             * =========================================================================
             * Task Status Codes (200–299)
             * =========================================================================
             */

        case GT_STATUS_TASK_LIMIT_REACHED:
            return "task limit reached";

        case GT_STATUS_TASK_NOT_FOUND:
            return "task not found";

            /*
             * =========================================================================
             * Platform Status Codes (600 - 699)
             * =========================================================================
             */

        case GT_STATUS_VM_RESERVATION_FAILED:
            return "vm mapping reservation failed";

        case GT_STATUS_VM_PROTECTION_FAILED:
            return "vm mapping protection failed";

        case GT_STATUS_VM_RELEASE_FAILED:
            return "vm mapping release failed";

        case GT_STATUS_PAGE_SIZE_UNAVAILABLE:
            return "page size unavailable";

            /*
             * =========================================================================
             * Unknown Status
             * =========================================================================
             */

        default:
            return "unknown status";
    }
}
