#include "DlgAddProductConfig.h"

#include<QMessageBox>
#include<QFileDialog>

DlgAddProductConfig::DlgAddProductConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgAddProductConfigClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgAddProductConfig::~DlgAddProductConfig()
{
	delete ui;
}

void DlgAddProductConfig::ini_ui()
{
}

void DlgAddProductConfig::ini_connect()
{
	QObject::connect(ui->pbt_saveProductConfig, SIGNAL(clicked()),
		this, SLOT(pbt_saveProductConfig_clicked()));
	QObject::connect(ui->pbtn_spinImage, SIGNAL(clicked()),
		this, SLOT(pbtn_spinImage_clicked()));
	QObject::connect(ui->pbtn_drawRecognitionRange, SIGNAL(clicked()),
		this, SLOT(pbtn_drawRecognitionRange_clicked()));
}

void DlgAddProductConfig::pbtn_spinImage_clicked()
{
	QMessageBox::information(this, "", "");
}

void DlgAddProductConfig::pbtn_drawRecognitionRange_clicked()
{
	QMessageBox::information(this, "", "");
}

void DlgAddProductConfig::pbt_saveProductConfig_clicked()
{
	QFileDialog 
		fileDlg(nullptr, tr("���ļ�"), 
			"", tr("�����ļ�(*.xml);;�����ļ� (*)"));
	if (fileDlg.exec() == QFileDialog::Accepted) {
		auto fileName = fileDlg.selectedFiles().first();
		QMessageBox::information(this,"",fileName);
	}
}
