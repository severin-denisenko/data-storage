#include <iostream>
#include <vector>

#include "DataBase.h"
#include "Logger.h"

int main()
{
    Logger::Init("./", "DataBase.log");

    DataBase data_base = DataBase("testdb");
    data_base.Put("key1", "data1");
    data_base.Put("key2", "data2");
    data_base.Put("key3", "data3");

    std::cout << data_base.Get("key1") << std::endl;
    std::cout << data_base.Get("key2") << std::endl;
    std::cout << data_base.Get("key3") << std::endl;

    std::vector keys = {"vector1", "vector2", "vector3"};
    std::vector values = {"data-vector1", "data-vector2", "data-vector3"};

    data_base.BachWrite(values.begin(), values.end(),
                        keys.begin(), keys.end());


    data_base.Iterate([](const std::string& str){
        LOG(INFO) << str << std::endl;
    });

    return 0;
}
