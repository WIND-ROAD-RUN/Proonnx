#include "Proonnx.h"
#include <QtWidgets/QApplication>

//#include <spdlog/spdlog.h>
//#include <spdlog/sinks/basic_file_sink.h>

//void iniLogger();

int main(int argc, char *argv[])
{
    //iniLogger();

    //spdlog::info("----------------------------------------");
    //spdlog::info("Create window Proonx");
    QApplication a(argc, argv);
    Proonnx w;
    w.show();

    return a.exec();
}

//void iniLogger()
//{
//    auto file_logger = spdlog::basic_logger_mt("file_logger", "logs/logDebug.txt");
//
//    spdlog::set_default_logger(file_logger);
//}
