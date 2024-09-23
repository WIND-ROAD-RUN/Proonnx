#pragma once

#include <QDialog>
#include "ui_DlgManageProductConfig.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DlgManageProductConfigClass; };
QT_END_NAMESPACE

class DlgManageProductConfig : public QDialog
{
    Q_OBJECT

public:
    DlgManageProductConfig(QWidget *parent = nullptr);
    ~DlgManageProductConfig();

private:
    Ui::DlgManageProductConfigClass *ui;
};
