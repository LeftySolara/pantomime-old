#include "mpdwrapper/songlist.h"
#include "mpdwrapper/mpdwrapper.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void test_init()
{
    struct mpdwrapper *mpd = mpdwrapper_init("localhost", 6600, 30000);
    struct mpd_song *current_song = mpd_run_current_song(mpd->connection);
    struct songnode *head = songnode_init(current_song);

    assert_non_null(head);
    assert_non_null(head->song);
    assert_null(head->prev);
    assert_null(head->next);

    songnode_free(head);
    mpdwrapper_free(mpd);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}