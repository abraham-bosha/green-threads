#pragma once

#include <stdint.h>

/*
 * Version encoding:
 *
 *   version = (major * 10000u) +
 *             (minor * 100u) +
 *             (patch)
 *
 * Constraints:
 *
 *   - minor: [0, 99]
 *   - patch: [0, 99]
 */

#define GT_VERSION_MAJOR 1
#define GT_VERSION_MINOR 0
#define GT_VERSION_PATCH 0

/*
 * @brief Unified base-10 integer token for fast preprocessor checks.
 * @note  Enables layout directives like: #if GT_VERSION_NUMBER >= 10000
 */
#define GT_VERSION_NUMBER                                                                          \
    ((GT_VERSION_MAJOR * 10000u) + (GT_VERSION_MINOR * 100u) + (GT_VERSION_PATCH))

#define GT_VERSION_STR_HELPER(x) #x
#define GT_VERSION_TO_STR(x) GT_VERSION_STR_HELPER(x)

/*
 * @brief The raw, compile-time string representation of the version grid.
 */
#define GT_VERSION_STRING                                                                          \
    GT_VERSION_TO_STR(GT_VERSION_MAJOR)                                                            \
    "." GT_VERSION_TO_STR(GT_VERSION_MINOR) "." GT_VERSION_TO_STR(GT_VERSION_PATCH)

/*
 * @brief Returns the packed semantic version number of the runtime.
 */
uint32_t gt_version_number(void);

/*
 * @brief Returns the semantic version string of the runtime.
 */
const char *gt_version_string(void);
