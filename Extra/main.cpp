#include<iostream>
#include"cfgl/cfgl_LocalizationStringLoader.h"
#include"spdlog/spdlog.h"

using namespace std;
using namespace rw::cfgl;

int main()
{
    //Use the default logger (stdout, multi-threaded, colored)
    // create a color multi-threaded logger
    /*auto console = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
    spdlog::get("console")->info("loggers can be retrieved from a global registry using the spdlog::get(logger_name)");*/
#ifdef _DEBUG
    cout << "DEBUG";
#endif // DEBUG
#ifdef NDEBUG
    cout << "RELEASE";
#endif // RELEASE

    return 0;
}