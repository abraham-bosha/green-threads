#include <gt/version.h>
#include <stdio.h>

int main(void)
{
    printf("GT version: %s (%u)\n",
           gt_version_string(),
           gt_version_number());

    printf("Compile-time: %s (%u)\n",
           GT_VERSION_STRING,
           GT_VERSION_NUMBER);

    return 0;
}
