#include <iostream>
#include <vector>

#include "DataBase.h"
#include "Logger.h"
#include "Serialize.h"

int main()
{
    Logger::Init("./Logs", "DataBase.log");

    DataBase data_base = DataBase("Databases/testdb");

    std::vector<double> data = {1, 3, 4, 5};

    data_base.Put("some shit", Serialize::Serialize(data));

    auto new_data = Serialize::Deserialize<std::vector<double>>(data_base.Get("some shit"));

    for(auto item: new_data){
        LOG(INFO) << item << std::endl;
    }

    data_base.Iterate([](const std::string& str){
        LOG(INFO) << str << std::endl;
    });

    return 0;
}
