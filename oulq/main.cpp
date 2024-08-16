#include"VirtualKeyboard.h"
#include <QtWidgets/QApplication>


int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    VirtualKeyboard w;
    w.show();

    return a.exec();
}

