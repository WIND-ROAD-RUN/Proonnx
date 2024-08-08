#include "Proonnx.h"
#include <QtWidgets/QApplication>

#include"LogRecorder.h"

int main(int argc, char *argv[])
{
    auto logRecorder = LogRecorder::getInstance();
    logRecorder->iniCom();

    logRecorder->info("***********************************************");
    logRecorder->info("Proonex                            operate");
    logRecorder->info("***********************************************");
    
    QApplication a(argc, argv);
    Proonnx w;
    w.show();

    return a.exec();
}


