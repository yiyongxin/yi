/***************************************
* brief:    具有颗粒度的内存池
* author:   易雍鑫
* date:     2018-04-02
****************************************/

#ifndef GRAIN_SIZE_POOL_H_
#define GRAIN_SIZE_POOL_H_

#include <boost/pool/pool.hpp>
#include <unordered_map>

#define GSPOOL_UNIT_SIZE 32

class gspool
{
public:
    gspool(unsigned int size = GSPOOL_UNIT_SIZE);
    ~gspool();
public:
    void* malloc(unsigned int size);
    bool free(void* data, unsigned int size);
private:
    void* empty;
    unsigned int _grain_size;
    std::unordered_map<unsigned int,boost::pool<>*> _pool_map;
};

#endif //GRAIN_SIZE_POOL_H_
