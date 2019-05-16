#include "malloc_test.cpp"
#include "libuv_test.cpp"
#include "co_test.cpp"

void test_src()
{
    // malloc_test::test();
    co_test();
    libuv_test::test();
}