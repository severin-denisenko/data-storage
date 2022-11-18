//
// Created by Severin on 16.11.2022.
//

#ifndef DATA_STORAGE_SERIAL_H
#define DATA_STORAGE_SERIAL_H

#include <string>
#include <alpaca/alpaca.h>

#include "Logger.h"

namespace Serial{
    /*
     * Container for Serializing / Deserializing any data
     */
    template <typename T>
    struct Data{
        T data;
    };

    /*
     * Takes any data with type T and returns std::string
     */
    template <typename T>
    std::string Serialize(const T& data){
        Data<T> d{data};

        std::vector<uint8_t> bytes;
        alpaca::serialize(d, bytes);

        std::string str(bytes.begin(), bytes.end());
        return str;
    }

    /*
     * Takes serialized string and return object
     */
    template <typename T>
    T Deserialize(const std::string& str){
        std::vector<uint8_t> bytes(str.begin(), str.end());

        std::error_code ec;
        auto data = alpaca::deserialize<Data<T>>(bytes, ec);
        if (ec) {
            LOG(WARNING) << ec << std::endl;
        }

        return data.data;
    }
}

#endif //DATA_STORAGE_SERIAL_H
