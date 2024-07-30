#include "DlgSelectCameraIndex.h"


#include<qdebug>
#include"LocalizationStringLoader-XML.h"
#include"ConfigBeforeRuntimeLoader.h"

DlgSelectCameraIndex::DlgSelectCameraIndex(QWidget *parent, int cameraCount)
	: QDialog(parent),m_cameraCount(cameraCount)
	, ui(new Ui::DlgSelectCameraIndexClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgSelectCameraIndex::~DlgSelectCameraIndex()
{
	delete ui;
}

void DlgSelectCameraIndex::setConfigBeforeRuntime(const QString& filePath)
{
	m_configBeforeRuntime = filePath;
}

void DlgSelectCameraIndex::setWindowSize(int wide, int height)
{
	this->setFixedSize(wide, height);
}

void DlgSelectCameraIndex::ini_ui()
{
	qDebug() << m_cameraCount;
	ui->spBox_cameraIndex->setMaximum(m_cameraCount);
	ini_localizationStringLoaderUI();
}

void DlgSelectCameraIndex::ini_localizationStringLoaderUI()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	ConfigBeforeRuntimeLoader configLoader;
	configLoader.loadFile(m_configBeforeRuntime.toStdString());
	loader->setLanguage(configLoader.readLanguage());

	ui->pbtn_ok->setText(QString::fromStdString(loader->getString("15")));
	ui->pbtn_cancel->setText(QString::fromStdString(loader->getString("16")));
	ui->label->setText(QString::fromStdString(loader->getString("14")));
	this->setWindowTitle(QString::fromStdString(loader->getString("14")));
}

void DlgSelectCameraIndex::ini_connect()
{
	QObject::connect(ui->pbtn_ok, SIGNAL(clicked()),
		this, SLOT(pbtn_ok_clicked()));
	QObject::connect(ui->pbtn_cancel, SIGNAL(clicked()),
		this, SLOT(pbtn_cancel()));
}

void DlgSelectCameraIndex::pbtn_ok_clicked()
{
	m_indexIndex = ui->spBox_cameraIndex->value();
	this->accept();
}

void DlgSelectCameraIndex::pbtn_cancel()
{
	this->reject();
}
