#include "Proonnx.h"

#include"LogRecorder.h"
#include"cfgr/cfgr_CatalogueInitializer.h"

#include <QtWidgets/QApplication>
#include<QInputMethod>
#include<QDir>


int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    auto logRecorder = LogRecorder::getInstance();
    logRecorder->iniCom();

    QApplication a(argc, argv);
    Proonnx w;
    w.show();

    return a.exec();
}

