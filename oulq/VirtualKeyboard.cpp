#include "VirtualKeyboard.h"

VirtualKeyboard::VirtualKeyboard(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VirtualKeyboardClass())
{
    ui->setupUi(this);
}

VirtualKeyboard::~VirtualKeyboard()
{
    delete ui;
}
