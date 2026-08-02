#include <stdio.h>
#include <stdlib.h>

#include <gt/gt.h>

int
main(void)
{
    puts("[example] Initializing runtime...");

    const gt_status_t status = gt_init();
    if (status != GT_STATUS_SUCCESS)
    {
        fprintf(stderr, "[error] Runtime initialization failed: %s\n", gt_status_string(status));
        return EXIT_FAILURE;
    }

    puts("[example] Runtime initialized successfully.");

    /*
     * No tasks are created in this example.
     * This simply demonstrates the runtime lifecycle.
     */

    puts("[example] Destroying runtime...");
    gt_destroy();

    puts("[example] Runtime shut down successfully.");

    return EXIT_SUCCESS;
}
