#include "mpdwrapper/songlist.h"
#include "mpdwrapper/mpdwrapper.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_init(void **state)
{
    struct songlist *list = songlist_init();

    assert_non_null(list);
    assert_null(list->head);
    assert_null(list->tail);
    assert_int_equal(list->size, 0);

    songlist_free(list);
}

static void test_append(void **state)
{
    struct mpd_connection *mpd = mpd_connection_new("localhost", 6600, 30000);
    struct mpd_song *song = mpd_run_current_song(mpd);
    struct songlist *list = songlist_init();

    /* Append when list is empty. */
    songlist_append(list, song);
    assert_non_null(list->head);
    assert_non_null(list->tail);
    assert_int_equal(list->size, 1);

    /* Append when list is not empty. */
    struct mpd_song *song2 = mpd_run_current_song(mpd);
    songlist_append(list, song2);

    assert_non_null(list->head);
    assert_non_null(list->head->next);
    assert_non_null(list->tail);
    assert_int_equal(list->size, 2);

    songlist_free(list);
    mpd_connection_free(mpd);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init),
        cmocka_unit_test(test_append)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}