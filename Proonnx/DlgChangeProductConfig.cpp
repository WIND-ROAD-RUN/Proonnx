#include "DlgChangeProductConfig.h"

#include<QMessageBox>

#include"spdlog/spdlog.h"
#include"ProductConfigLoader.h"
#include"FrameSelectLabel.h"
#include"ImageIdentify.h"
#include"LocalizationStringLoader-XML.h"
#include"ConfigBeforeRuntimeLoader.h"

void DlgChangeProductConfig::setConfigBeforeRuntime(const QString& filePath)
{
	m_configBeforeRuntime = filePath;
}

void DlgChangeProductConfig::setCamera(ImageIdentify* camera)
{
	m_camera = camera;
	m_camera->setDlgLabelForImage(m_frameSelectLabel);
}

DlgChangeProductConfig::DlgChangeProductConfig(QWidget* parent, int cameraIndex)
	: QDialog(parent)
	, ui(new Ui::DlgChangeProductConfigClass())
{
	ui->setupUi(this);
	ini_ui();
	ini_connect();
}

DlgChangeProductConfig::~DlgChangeProductConfig()
{
	if (m_camera) {
		m_camera->deleteDlgLabelForImage();
	}
	delete ui;
	delete m_loader;
	delete m_recognizeRange;
	delete m_frameSelectLabel;
}

void DlgChangeProductConfig::ini_ui()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	ui->lEdit_filePath->setEnabled(false);
	m_frameSelectLabel = new FrameSelectLabel();
	m_frameSelectLabel->setText(QString::fromStdString(loader->getString("21")));
	QVBoxLayout* gBox_dispalyImageLayout = new QVBoxLayout();
	gBox_dispalyImageLayout->addWidget(m_frameSelectLabel);
	ui->gBox_dispalyImage->setLayout(gBox_dispalyImageLayout);
	m_frameSelectLabel->setScaledContents(true);
	ini_localizationStringLoaderUI();
}

void DlgChangeProductConfig::ini_localizationStringLoaderUI()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	ConfigBeforeRuntimeLoader configLoader;
	configLoader.loadFile(m_configBeforeRuntime.toStdString());
	loader->setLanguage(configLoader.readLanguage());

	ui->label_productName->setText(QString::fromStdString(loader->getString("7")));
	ui->label_drawRecognitionRange->setText(QString::fromStdString(loader->getString("8")));
	ui->label_gain->setText(QString::fromStdString(loader->getString("9")));
	ui->pbtn_spinImage->setText(QString::fromStdString(loader->getString("10")));
	ui->pbtn_drawRecognitionRange->setText(QString::fromStdString(loader->getString("11")));
	ui->pbt_saveProductConfig->setText(QString::fromStdString(loader->getString("12")));
	ui->label_filePath->setText(QString::fromStdString(loader->getString("13")));
	this->setWindowTitle(QString::fromStdString(loader->getString("6")));
}

void DlgChangeProductConfig::ini_connect()
{
	QObject::connect(ui->pbt_saveProductConfig, SIGNAL(clicked()),
		this, SLOT(pbt_saveProductConfig_clicked()));
	QObject::connect(ui->pbtn_spinImage, SIGNAL(clicked()),
		this, SLOT(pbtn_spinImage_clicked()));
	QObject::connect(ui->pbtn_drawRecognitionRange, SIGNAL(clicked()),
		this, SLOT(pbtn_drawRecognitionRange_clicked()));
	QObject::connect(m_frameSelectLabel, SIGNAL(selectionMade(const QRect&)),
		this, SLOT(selectionMade_complete(const QRect&)));
	QObject::connect(ui->sBox_exposureTime, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_exposureTime_value_change(int)));
	QObject::connect(ui->sBox_gain, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_gain_value_change(int)));
}

void DlgChangeProductConfig::ini_configLoader()
{
	m_loader = new ProductConfigLoader();
	m_recognizeRange = new RecognizeRange();
	auto productConfig = m_loader->loadConfig(m_filePath.toStdString());
	ui->lEdit_productName->setText(QString::fromStdString(productConfig.productName));
	ui->sBox_exposureTime->setValue(productConfig.ExposureTime);
	ui->sBox_gain->setValue(productConfig.gain);
	m_rotateCount = productConfig.rotateCount;

	QRect lastRange;
	m_recognizeRange->leftLowerCorner = productConfig.leftLowerCorner;
	lastRange.setBottomLeft(QPoint(productConfig.leftLowerCorner.first, productConfig.leftLowerCorner.second));

	m_recognizeRange->lowerRightCorner = productConfig.lowerRightCorner;
	lastRange.setBottomRight(QPoint(productConfig.lowerRightCorner.first, productConfig.lowerRightCorner.second));

	m_recognizeRange->topLeftCorner = productConfig.topLeftCorner;
	lastRange.setTopLeft(QPoint(productConfig.topLeftCorner.first, productConfig.topLeftCorner.second));

	m_recognizeRange->upperRightCorner = productConfig.upperRightCorner;
	lastRange.setTopRight(QPoint(productConfig.upperRightCorner.first, productConfig.upperRightCorner.second));

	m_frameSelectLabel->setLastSelectionRect(lastRange);
}

void DlgChangeProductConfig::iniUI()
{
	ini_configLoader();
	ui->lEdit_filePath->setText(m_filePath);
}

void DlgChangeProductConfig::pbt_saveProductConfig_clicked()
{
	auto loader = LocalizationStringLoaderXML::getInstance();
	if (ui->lEdit_productName->text().size() == 0) {
		QMessageBox::warning(this, QString::fromStdString(loader->getString("34")), QString::fromStdString(loader->getString("35")));
		return;
	}

	ProductConfigLoader configLoader;
	configLoader.setNewFile(m_filePath.toStdString());

	ProductConfig config;
	config.gain = ui->sBox_gain->value();
	config.productName = ui->lEdit_productName->text().toStdString();
	config.rotateCount = m_rotateCount;
	config.ExposureTime = ui->sBox_exposureTime->value();
	config.topLeftCorner = m_recognizeRange->topLeftCorner;
	config.leftLowerCorner = m_recognizeRange->leftLowerCorner;
	config.upperRightCorner = m_recognizeRange->upperRightCorner;
	config.lowerRightCorner = m_recognizeRange->lowerRightCorner;

	auto storeConfigResult = configLoader.storeConfig(config);
	auto saveConfigResult = configLoader.saveFile(m_filePath.toStdString());
	if (storeConfigResult && saveConfigResult) {
		QMessageBox::information(this, QString::fromStdString(loader->getString("12")), QString::fromStdString(loader->getString("24")));
	}
	else {
		QMessageBox::warning(this, QString::fromStdString(loader->getString("12")), QString::fromStdString(loader->getString("25")));
	}

	this->accept();
}

void DlgChangeProductConfig::pbtn_spinImage_clicked()
{
	m_rotateCount++;
	m_rotateCount = m_rotateCount % 4;
}

void DlgChangeProductConfig::pbtn_drawRecognitionRange_clicked()
{
	m_frameSelectLabel->enableSelection(true);
}

void DlgChangeProductConfig::sBox_exposureTime_value_change(int)
{
	m_camera->stopAcquisition();
	m_camera->setExposureTime(ui->sBox_exposureTime->value());
	m_camera->startAcquisition();
}

void DlgChangeProductConfig::sBox_gain_value_change(int)
{
	m_camera->stopAcquisition();
	m_camera->setGain(ui->sBox_gain->value());
	m_camera->startAcquisition();
}

void DlgChangeProductConfig::selectionMade_complete(const QRect& rect)
{
	m_recognizeRange->leftLowerCorner = { rect.bottomLeft().x(),rect.bottomLeft().y() };
	m_recognizeRange->topLeftCorner = { rect.topLeft().x(),rect.topLeft().y() };
	m_recognizeRange->lowerRightCorner = { rect.bottomRight().x(),rect.bottomRight().y() };
	m_recognizeRange->upperRightCorner = { rect.topRight().x(),rect.topRight().y() };
	m_frameSelectLabel->enableSelection(false);
}
