/***************************************
* brief:    可变长数据读写功能
* author:   易雍鑫
* date:     2018-03-31
****************************************/

#ifndef VARY_LEN_DATA_H_
#define VARY_LEN_DATA_H_

namespace varyLen
{
    int readInt(void *data,int &len);    //返回一个从变长数据中解析出的int数据，并把读取的长度写入len
    int writeInt(void *data,int num);    //将一个int数据以变长形式写入数据块，并返回写入数据块的长度
}

#endif //VARY_LEN_DATA_H_