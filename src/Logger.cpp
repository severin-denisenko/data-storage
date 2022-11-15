//
// Created by Severin on 16.11.2022.
//

#include "Logger.h"

std::unique_ptr<g3::LogWorker> Logger::s_worker;
std::unique_ptr<g3::FileSinkHandle> Logger::s_handle;

void Logger::Init(const std::string& t_directory, const std::string& t_name)
{
    s_worker = g3::LogWorker::createLogWorker();
    s_handle = s_worker->addDefaultLogger(t_name, t_directory);

    g3::initializeLogging(s_worker.get());
}
