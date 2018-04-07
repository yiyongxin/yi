/***************************************
* brief:    具有颗粒度的内存池
* author:   易雍鑫
* date:     2018-04-02
****************************************/

#include "grainSizePool.h"

gspool::gspool(unsigned int size):_grain_size(size)
{
    empty = (void*)new char;
}

gspool::~gspool()
{
    for(auto it=_pool_map.begin();it!=_pool_map.end();it++)
    {
        void* temp = it->second;
        if(temp != NULL)
            delete temp;
    }
    delete empty;
}

void* gspool::malloc(unsigned int size)
{
    if(size == 0)
        return empty;    
    size--;
    unsigned int index = size / _grain_size;
    auto it = _pool_map.find(index);
    boost::pool<>* tpool = NULL;
    if(it == _pool_map.end())
    {
        tpool = new boost::pool<>(_grain_size * (index +1));
        _pool_map.insert(std::make_pair(index,tpool));
    }
    else
        tpool = it->second;
    if(tpool == NULL)
        return NULL;
    return tpool->malloc();
}

bool gspool::free(void* data, unsigned int size)
{
    if(size == 0)
        return true;
    size--;
    unsigned int index = size / _grain_size;
    auto it = _pool_map.find(index);
    if(it == _pool_map.end())
        return false;
    boost::pool<>* tpool = it->second;
    if(tpool == NULL)
        return false;
    tpool->free(data);
    return true;
}