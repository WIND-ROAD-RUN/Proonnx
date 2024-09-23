#include "DlgManageProductConfig.h"

DlgManageProductConfig::DlgManageProductConfig(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgManageProductConfigClass())
{
    ui->setupUi(this);
}

DlgManageProductConfig::~DlgManageProductConfig()
{
    delete ui;
}
