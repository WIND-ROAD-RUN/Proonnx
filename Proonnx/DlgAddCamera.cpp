#include "DlgAddCamera.h"

DlgAddCamera::DlgAddCamera(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgAddCameraClass())
{
	ui->setupUi(this);
}

DlgAddCamera::~DlgAddCamera()
{
	delete ui;
}
