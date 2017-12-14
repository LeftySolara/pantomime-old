#include <stdio.h>
#include "PantomimeConfig.h"

int main(int argc, char **argv)
{
    fprintf(stdout,
            "Pantomime Version %d.%d\n",
            Pantomime_VERSION_MAJOR,
            Pantomime_VERSION_MINOR);

    return 0;
}
