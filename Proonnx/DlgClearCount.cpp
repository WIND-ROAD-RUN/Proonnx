#include "DlgClearCount.h"

#include"LocalizationStringLoader-XML.h"
#include<QVBoxLayout>
#include<QCheckBox>
DlgClearCount::DlgClearCount(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgClearCountClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_localizationStringUI();
	ini_connect();
}

DlgClearCount::~DlgClearCount()
{
	delete ui;
}

void DlgClearCount::ini_ui()
{

}

void DlgClearCount::ini_localizationStringUI()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	this->setWindowTitle(QString::fromStdString(loader->getString("36")));
	ui->pbtn_ok->setText(QString::fromStdString(loader->getString("15")));
	ui->pbtn_cancel->setText(QString::fromStdString(loader->getString("16")));
}

void DlgClearCount::ini_connect()
{
	QObject::connect(ui->pbtn_ok, SIGNAL(clicked()),
		this, SLOT(pbtn_ok_clicked()));
	QObject::connect(ui->pbtn_cancel, SIGNAL(clicked()),
		this, SLOT(pbtn_cancel_clicked()));
}

void DlgClearCount::iniGBox_cameraList(int cameraCount)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	for (int i = 0; i < cameraCount; i++) {
		QCheckBox* cBox = new QCheckBox();
		cBox->setText(QString::number(i + 1));
		auto sizePolicy = cBox->sizePolicy();
		sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
		sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
		cBox->setSizePolicy(sizePolicy);
		layout->addWidget(cBox);
	}
	ui->gBox_cameraList->setLayout(layout);
}

QVector<bool> DlgClearCount::getIsCheckList()
{
	QVector<bool> result;
	auto layout = ui->gBox_cameraList->layout();

	for (int i = 0; i < layout->count(); ++i) {
		QCheckBox* checkBox = qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
		if (checkBox) {
			result.append(checkBox->isChecked());
		}
	}

	return result;
}

void DlgClearCount::setWindowSize(int wide, int height)
{
	this->setFixedSize(wide, height);
}

void DlgClearCount::pbtn_ok_clicked()
{
	this->accept();
}

void DlgClearCount::pbtn_cancel_clicked()
{
	this->reject();
}
