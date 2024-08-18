#include "DlgSetIsCheckProduct.h"

#include"LocalizationStringLoader-XML.h"
#include<QVBoxLayout>
#include<QCheckBox>

DlgSetIsCheckProduct::DlgSetIsCheckProduct(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgSetIsCheckProductClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_localizationStringUI();
	ini_connect();
}

DlgSetIsCheckProduct::~DlgSetIsCheckProduct()
{
	delete ui;
}

void DlgSetIsCheckProduct::ini_ui()
{
}

void DlgSetIsCheckProduct::ini_localizationStringUI()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	this->setWindowTitle(QString::fromStdString(loader->getString("33")));
	ui->pbtn_ok->setText(QString::fromStdString(loader->getString("15")));
	ui->pbtn_cancel->setText(QString::fromStdString(loader->getString("16")));
}

void DlgSetIsCheckProduct::ini_connect()
{
	QObject::connect(ui->pbtn_ok, SIGNAL(clicked()),
		this, SLOT(pbtn_ok_clicked()));
	QObject::connect(ui->pbtn_cancel, SIGNAL(clicked()),
		this, SLOT(pbtn_cancel_clicked()));
}

void DlgSetIsCheckProduct::iniGBox_cameraList(const QVector<bool>& list)
{
	m_isCheckedlist = list;
	QVBoxLayout* layout = new QVBoxLayout(this);
	for (int i = 0;i<list.size();i++) {
		QCheckBox* cBox = new QCheckBox();
		cBox->setText(QString::number(i+1));
		auto sizePolicy=cBox->sizePolicy();
		cBox->setChecked(list[i]);
		sizePolicy.setHorizontalPolicy(QSizePolicy::Expanding);
		sizePolicy.setVerticalPolicy(QSizePolicy::Expanding);
		cBox->setSizePolicy(sizePolicy);
		layout->addWidget(cBox);
	}
	ui->gBox_cameraList->setLayout(layout);
}

QVector<bool> DlgSetIsCheckProduct::getCurrentIsCheckList()
{
	QVector<bool> result;
	auto layout=ui->gBox_cameraList->layout();

	for (int i = 0; i < layout->count(); ++i) {
		QCheckBox* checkBox = qobject_cast<QCheckBox*>(layout->itemAt(i)->widget());
		if (checkBox) {
			result.append(checkBox->isChecked());
		}
	}

	return result;
}

void DlgSetIsCheckProduct::setWindowSize(int wide, int height)
{
	this->setFixedSize(wide, height);
}

void DlgSetIsCheckProduct::pbtn_ok_clicked()
{
	this->accept();
}

void DlgSetIsCheckProduct::pbtn_cancel_clicked()
{
	this->reject();
}
