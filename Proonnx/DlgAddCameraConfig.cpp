#include "DlgAddCameraConfig.h"

DlgAddCameraConfig::DlgAddCameraConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgAddCameraConfigClass())
{
	ui->setupUi(this);
}

DlgAddCameraConfig::~DlgAddCameraConfig()
{
	delete ui;
}
