#include <experimental/coroutine>

generator<int> co_test()
{
    for( int current=0; current<10;current++)
        co_yield current;
}