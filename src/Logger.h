//
// Created by Severin on 16.11.2022.
//

#ifndef DATA_STORAGE_LOGGER_H
#define DATA_STORAGE_LOGGER_H

#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>

class Logger
{
public:
    Logger() = delete;

    /**
     * @details Initializes logger. Must be cold before first usage.
     * @param t_directory Place to put log files.
     * @param t_name Name of the log file. Final name will be t_directory/t_name.g3log.year-mouth-day-time.log
     */
    static void Init(const std::string& t_directory, const std::string& t_name);
private:
    static std::unique_ptr<g3::LogWorker> s_worker;
    static std::unique_ptr<g3::FileSinkHandle> s_handle;
};


#endif //DATA_STORAGE_LOGGER_H
