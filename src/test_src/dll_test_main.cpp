// #include <stdio.h>
// #include <stdlib.h>
// #include <dlfcn.h>

// typedef void*(*Test)();

// int main(int argc, char *argv[])
// {
//     void *handle = dlopen("./lib/libfun.so", RTLD_LAZY);
//     Test testptr = (Test)dlsym(handle,"testptr");
//     if(testptr == NULL)
//         printf("err dlsym\n");
//     int *i = NULL;
//     i = (int*)testptr();
//     printf("i: %d\n", *i);
//     delete i;
//     printf("i: %d\n", *i);
//     return 0;
// }