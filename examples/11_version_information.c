#include <stdint.h>
#include <stdio.h>

#include <gt/gt.h>

int
main(void)
{
    printf("[example] Green Threads Version Information\n\n");

    printf("Compile-time version:\n");
    printf("  String : %s\n", GT_VERSION_STRING);
    printf("  Number : %u\n", GT_VERSION_NUMBER);
    printf("  Major  : %d\n", GT_VERSION_MAJOR);
    printf("  Minor  : %d\n", GT_VERSION_MINOR);
    printf("  Patch  : %d\n\n", GT_VERSION_PATCH);

    printf("Runtime version:\n");

    const char *runtime_string = gt_version_string();
    uint32_t runtime_number = gt_version_number();

    printf("  String : %s\n", runtime_string);
    printf("  Number : %u\n\n", runtime_number);

    printf("Compatibility check:\n");

    if (runtime_number != GT_VERSION_NUMBER)
    {
        fprintf(stderr, "  [FAIL] Header and runtime versions do not match.\n");
        return 1;
    }

    printf("  [PASS] Header and runtime versions match.\n");

    return 0;
}
