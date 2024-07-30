#include"LogModule.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

void LogFileModule::iniCom()
{
    
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    
    std::tm local_time;
    localtime_s(&local_time, &now_c); 

    
    char buffer[11]; 
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &local_time);

    std::string fileName;

    fileName = std::string("logs/") + std::string(buffer) + std::string(".txt");

	auto file_logger = spdlog::basic_logger_mt("file_logger", fileName);

	spdlog::set_default_logger(file_logger);
}

void LogFileModule::info(const std::string& str)
{
	spdlog::info(str);
}

void LogFileModule::error(const std::string& str)
{
	spdlog::error(str);
}

void LogFileModule::warn(const std::string& str)
{
	spdlog::warn(str);
}
