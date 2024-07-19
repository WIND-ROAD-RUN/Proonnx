#include "DlgChangeProductConfig.h"

DlgChangeProductConfig::DlgChangeProductConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgChangeProductConfigClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgChangeProductConfig::~DlgChangeProductConfig()
{
	delete ui;
}

void DlgChangeProductConfig::ini_ui()
{
	ui->lEdit_filePath->setEnabled(false);
}

void DlgChangeProductConfig::ini_connect()
{
}

void DlgChangeProductConfig::iniUI()
{
	ui->lEdit_filePath->setText(m_filePath);
}
