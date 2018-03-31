/***************************************
* brief:    可变长数据读写功能
* author:   易雍鑫
* date:     2018-03-31
****************************************/

#include "varyLenData.h"

int varyLen::readInt(void *data,int &len)
{
    int rint = 0;
    for(len=0;len<5;len++)
    {
        int temp = (int)*(char*)(data + len);
        rint = rint | (temp & 0x7f << (7*len));
        if(temp & 0x80)
            break;
    }
    len++;
    return rint;
}

int varyLen::writeInt(void *data,int num)
{
    int len;
    for(len=0;len<5;len++)
    {
        void* temp = data + len;
        *(char*)temp = num & 0x7f | 0x80;
        num >> 7;
        if(num == 0)
            break;
    }
    return len++;
}