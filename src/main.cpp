#include <iostream>
#include <vector>

#include "DataBase.h"
#include "Logger.h"

#include <alpaca/alpaca.h>

struct Config {
    std::string device;
    std::pair<unsigned, unsigned> resolution;
    std::array<double, 9> K_matrix;
    std::vector<float> distortion_coeffients;
    std::map<std::string, std::variant<uint16_t, std::string, bool>> parameters;
};

int main()
{
    Logger::Init("./Logs", "DataBase.log");

    DataBase data_base = DataBase("Databases/testdb");

    // Construct the object
    Config c{"/dev/video0", {640, 480},
             {223.28249888247538, 0.0, 152.30570853111396,
              0.0, 223.8756535707556, 124.5606000035353,
              0.0, 0.0, 1.0},
             {-0.44158343539568284, 0.23861463831967872, 0.0016338407443826572,
              0.0034950038632981604, -0.05239245892096022},
             {{"start_server", bool{true}},
              {"max_depth", uint16_t{5}},
              {"model_path", std::string{"foo/bar.pt"}}}};

    // Serialize
    std::vector<uint8_t> bytes;
    alpaca::serialize(c, bytes);
    std::string shit(bytes.begin(), bytes.end());

    // Put in DB
    data_base.Put("some shit", shit);

    // Get from DB
    std::string new_shit = data_base.Get("some shit");

    // Deserialize
    std::vector<uint8_t> new_bytes(new_shit.begin(), new_shit.end());
    std::error_code ec;
    auto object = alpaca::deserialize<Config>(bytes, ec);
    if (ec) {
        LOG(WARNING) << ec << std::endl;
    }

    data_base.Iterate([](const std::string& str){
        LOG(INFO) << str << std::endl;
    });

    return 0;
}
