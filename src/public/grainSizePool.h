/***************************************
* brief:    具有颗粒度的内存池
* author:   易雍鑫
* date:     2018-04-02
****************************************/

#ifndef GRAIN_SIZE_POOL_H_
#define GRAIN_SIZE_POOL_H_

#include <boost/pool/pool.hpp>
#include <vector>

class gspool
{
public:
    gspool(int size = 128);
    ~gspool();
public:
    void* malloc(int size);
    void free(void* data, int size);
private:
    int _grain_size;
    std::vector<void*> _write_pool_array;
};

#endif //GRAIN_SIZE_POOL_H_