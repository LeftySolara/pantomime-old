#include "mpdwrapper/mpdwrapper.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_connection()
{
    struct mpdwrapper *mpd = mpdwrapper_new("localhost", 6600, 30000);
    mpdwrapper_refresh(mpd);
    assert_int_equal(mpd->last_error, MPD_ERROR_SUCCESS);
    mpdwrapper_free(mpd);
}

static void test_fetch_queue()
{
    struct mpdwrapper *mpd = mpdwrapper_new("localhost", 6600, 30000);
    mpdwrapper_refresh(mpd);
    int queue_length = mpd_status_get_queue_length(mpd->status);

    mpdwrapper_fetch_queue(mpd);
    assert_int_equal(queue_length, mpd->queue->size);

    mpdwrapper_free(mpd);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_connection),
        cmocka_unit_test(test_fetch_queue)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
