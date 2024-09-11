#include"oulq_DialogCustom.h"
#include <QtWidgets/QApplication>

using namespace rw::oulq;

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    DialogInitialization dlg;
    dlg.show();
    return a.exec();
}