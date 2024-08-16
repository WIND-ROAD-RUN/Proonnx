#pragma once

#include <QDialog>
#include "ui_VirtualKeyboard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class VirtualKeyboardClass; };
QT_END_NAMESPACE

class VirtualKeyboard : public QDialog
{
    Q_OBJECT

public:
    VirtualKeyboard(QWidget *parent = nullptr);
    ~VirtualKeyboard();

private:
    Ui::VirtualKeyboardClass *ui;
};
