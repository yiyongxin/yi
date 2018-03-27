extern "C"
{
void *testptr();
}

void *testptr()
{
    int *i = new int;
    *i = 155;
    return i;
}