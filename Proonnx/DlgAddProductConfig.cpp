#include "DlgAddProductConfig.h"

DlgAddProductConfig::DlgAddProductConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgAddProductConfigClass())
{
	ui->setupUi(this);
}

DlgAddProductConfig::~DlgAddProductConfig()
{
	delete ui;
}
