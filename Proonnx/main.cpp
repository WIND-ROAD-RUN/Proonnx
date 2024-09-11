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

    logRecorder->info("***********************************************");
    logRecorder->info("Proonnx                            operate");
    logRecorder->info("***********************************************");

    rw::cfgr::CatalogueInitializer catalogueIni;
    auto  currentFilePath = QDir::currentPath();
    catalogueIni.setRootPath(R"(C:\Users\WINDROAD\Desktop\test)");
    catalogueIni.createDirectory("Config");
    catalogueIni.createDirectory("ProductConfig");
    catalogueIni.createDirectory("HistoryImage");

    QApplication a(argc, argv);
    Proonnx w;
    w.show();

    return a.exec();
}

