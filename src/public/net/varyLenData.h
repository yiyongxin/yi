
namespace varyLen
{
    int readInt(void *data,int &len);    //返回一个从变长数据中解析出的int数据，并把读取的长度写入len
    int writeInt(void *data,int num);    //将一个int数据以变长形式写入数据块，并返回写入数据块的长度
}