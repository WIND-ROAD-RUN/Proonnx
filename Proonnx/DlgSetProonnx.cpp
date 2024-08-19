#include "DlgSetProonnx.h"

#include"ConfigBeforeRuntimeLoader.h"
#include"cfgl/cfgl_LocalizationStringLoader.h"

#include<QDir>

DlgSetProonnx::DlgSetProonnx(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgSetProonnxClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgSetProonnx::~DlgSetProonnx()
{
	delete ui;
	delete m_configBeforeRuntimeLoader;
}

void DlgSetProonnx::iniComponet()
{
	ini_configBeforeRuntimeLoader(m_filePath.toStdString());
}

void DlgSetProonnx::setWindowSize(int wide, int height)
{
	this->setFixedSize(wide, height);
}

void DlgSetProonnx::ini_ui()
{
	ini_localizationStringUI();
}

void DlgSetProonnx::ini_configBeforeRuntimeLoader(const std::string& filePath)
{
	m_configBeforeRuntimeLoader = new ConfigBeforeRuntimeLoader();

	auto loadResult = m_configBeforeRuntimeLoader->loadFile(filePath);

	ini_uiFromConfigBeforeRuntime();
}

void DlgSetProonnx::ini_uiFromConfigBeforeRuntime()
{
	ui->sBox_cameraCount->setValue(m_configBeforeRuntimeLoader->readCameraCount());
	if (m_configBeforeRuntimeLoader->readLanguage()=="CHN") {
		ui->coBox_language->setCurrentIndex(0);
	}
	else if(m_configBeforeRuntimeLoader->readLanguage() == "USA"){
		ui->coBox_language->setCurrentIndex(1);
	}
	else {
		ui->coBox_language->setCurrentIndex(0);
	}
}

void DlgSetProonnx::ini_localizationStringUI()
{
	m_locStrLoader = LocalizationStringLoaderXML::getInstance();
	ConfigBeforeRuntimeLoader configLoader;
	configLoader.loadFile(m_filePath.toStdString());
	m_locStrLoader->setLanguage(configLoader.readLanguage());

	ui->label_language->setText(QString::fromStdString(m_locStrLoader->getString("17")));
	ui->label_cameraCount->setText(QString::fromStdString(m_locStrLoader->getString("18")));
	ui->pbtn_accpet->setText(QString::fromStdString(m_locStrLoader->getString("15")));
	ui->pbtn_cancel->setText(QString::fromStdString(m_locStrLoader->getString("16")));
	this->setWindowTitle(QString::fromStdString(m_locStrLoader->getString("20")));
}


void DlgSetProonnx::ini_connect()
{
	QObject::connect(ui->pbtn_accpet, SIGNAL(clicked()),
		this, SLOT(pbtn_accept_clicked()));
	QObject::connect(ui->coBox_language, SIGNAL(currentIndexChanged(int)),
		this, SLOT(cBox_changeLanguage_index_change_on(int)));
	QObject::connect(ui->pbtn_cancel, SIGNAL(clicked()),
		this, SLOT(pbtn_cancel_clicked()));

}

void DlgSetProonnx::cBox_changeLanguage_index_change_on(int index)
{
	if (index == 0) {
		m_locStrLoader->setLanguage("CHN");
		auto loadStrDataResult = m_locStrLoader->loadData();
		m_configBeforeRuntimeLoader->storeLanguage("CHN");
	}
	else if (index == 1) {
		m_locStrLoader->setLanguage("USA");
		auto loadStrDataResult = m_locStrLoader->loadData();
		m_configBeforeRuntimeLoader->storeLanguage("USA");
	}
	ini_localizationStringUI();
}

void DlgSetProonnx::pbtn_accept_clicked()
{
	m_configBeforeRuntimeLoader->storeCameraCount(ui->sBox_cameraCount->value());
	m_configBeforeRuntimeLoader->saveFile(m_filePath.toStdString());
	this->accept();
}

void DlgSetProonnx::pbtn_cancel_clicked()
{
	this->reject();
}
