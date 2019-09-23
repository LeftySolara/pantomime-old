#include "../src/mpdwrapper.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_connection()
{
    struct mpdwrapper *mpd = mpdwrapper_init("localhost", 6600, 30000);
    assert_int_equal(mpd->last_error, MPD_ERROR_SUCCESS);
    mpdwrapper_free(mpd);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_connection)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}