include_directories("${PROJECT_SOURCE_DIR}/src")

# mpdwrapper tests
add_executable(test_mpdwrapper test_mpdwrapper.c)
add_dependencies(test_mpdwrapper test_mpdwrapper)
add_test(test_mpdwrapper test_mpdwrapper)
add_test(test_mpdwrapper_valgrind valgrind
         --error-exitcode=1 --read-var-info=yes
         --leak-check=full --show-leak-kinds=all
         ./test_mpdwrapper)
target_link_libraries(test_mpdwrapper mpdwrapper mpdclient cmocka)