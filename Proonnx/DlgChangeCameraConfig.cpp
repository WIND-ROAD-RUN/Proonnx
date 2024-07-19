#include "DlgChangeCameraConfig.h"

DlgChangeCameraConfig::DlgChangeCameraConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgChangeCameraConfigClass())
{
	ui->setupUi(this);
}

DlgChangeCameraConfig::~DlgChangeCameraConfig()
{
	delete ui;
}
