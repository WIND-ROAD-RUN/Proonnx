#include "DlgAddProductConfig.h"

#include<QMessageBox>
#include<QFileDialog>
#include<QGroupBox>

#include"FrameSelectLabel.h"
#include"ProductConfigLoader.h"
#include"spdlog/spdlog.h"
#include"ImageIdentify.h"
#include"ConfigBeforeRuntimeLoader.h"
#include"LocalizationStringLoader-XML.h"



DlgAddProductConfig::DlgAddProductConfig(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::DlgAddProductConfigClass())
{
	ui->setupUi(this);
	m_recognizeRange = new RecognizeRange();
	spdlog::info("Iniliazing ui for dlgAddProductConfig");
	ini_ui();
	ini_connect();
}

DlgAddProductConfig::~DlgAddProductConfig()
{
	m_camera->deleteDlgLabelForImage();
	delete ui;
	delete m_frameSelectLabel;
	delete m_recognizeRange;
}

void DlgAddProductConfig::setCamera(ImageIdentify* camera)
{
	m_camera = camera;
	m_camera->setDlgLabelForImage(m_frameSelectLabel);
}

void DlgAddProductConfig::setConfigBeforeRuntime(const QString& filePath)
{
	m_configBeforeRuntime = filePath;
}

void DlgAddProductConfig::ini_ui()
{
	m_frameSelectLabel = new FrameSelectLabel;
	m_frameSelectLabel->setText("disconnect");
	QVBoxLayout* gBox_dispalyImageLayout = new QVBoxLayout();
	gBox_dispalyImageLayout->addWidget(m_frameSelectLabel);
	ui->gBox_dispalyImage->setLayout(gBox_dispalyImageLayout);
	m_frameSelectLabel->setScaledContents(true);
	ini_localizationStringLoaderUI();
}

void DlgAddProductConfig::ini_localizationStringLoaderUI()
{
	auto loader=LocalizationStringLoaderXML::getInstance();
	ConfigBeforeRuntimeLoader configLoader;
	configLoader.loadFile(m_configBeforeRuntime.toStdString());
	loader->setLanguage(configLoader.readLanguage());

	ui->label_productName->setText(QString::fromStdString(loader->getString("7")));
	ui->label_drawRecognitionRange->setText(QString::fromStdString(loader->getString("8")));
	ui->label_gain->setText(QString::fromStdString(loader->getString("9")));
	ui->pbtn_spinImage->setText(QString::fromStdString(loader->getString("10")));
	ui->pbtn_drawRecognitionRange->setText(QString::fromStdString(loader->getString("11")));
	ui->pbt_saveProductConfig->setText(QString::fromStdString(loader->getString("12")));
	this->setWindowTitle(QString::fromStdString(loader->getString("5")));
	
}

void DlgAddProductConfig::ini_connect()
{
	QObject::connect(ui->pbt_saveProductConfig, SIGNAL(clicked()),
		this, SLOT(pbt_saveProductConfig_clicked()));
	QObject::connect(ui->pbtn_spinImage, SIGNAL(clicked()),
		this, SLOT(pbtn_spinImage_clicked()));
	QObject::connect(ui->pbtn_drawRecognitionRange, SIGNAL(clicked()),
		this, SLOT(pbtn_drawRecognitionRange_clicked()));
	QObject::connect(m_frameSelectLabel, SIGNAL(selectionMade(const QRect & )),
		this, SLOT(selectionMade_complete(const QRect & )));
	QObject::connect(ui->sBox_exposureTime, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_exposureTime_value_change(int)));
	QObject::connect(ui->pbtn_spinImage, SIGNAL(valueChanged(int))
		, this, SLOT(sBox_gain_value_change(int)));
}

void DlgAddProductConfig::selectionMade_complete(const QRect& rect)
{
	m_recognizeRange->leftLowerCorner = { rect.bottomLeft().x(),rect.bottomLeft().y() };
	m_recognizeRange->topLeftCorner = { rect.topLeft() .x(),rect.topLeft() .y()};
	m_recognizeRange->lowerRightCorner = { rect.bottomRight().x(),rect.bottomRight().y() };
	m_recognizeRange->upperRightCorner = { rect.topRight().x(),rect.topRight().y() };
	m_frameSelectLabel->enableSelection(false);
	//qDebug() << "Selection made:" << rect;
	//qDebug() << "Top-left:" << rect.topLeft();
	//qDebug() << "Top-right:" << rect.topRight();
	//qDebug() << "Bottom-left:" << rect.bottomLeft();
	//qDebug() << "Bottom-right:" << rect.bottomRight();
}

void DlgAddProductConfig::sBox_exposureTime_value_change(int)
{
	m_camera->stopAcquisition();
	m_camera->setExposureTime(ui->sBox_exposureTime->value());
	m_camera->startAcquisition();
}

void DlgAddProductConfig::sBox_gain_value_change(int)
{
	m_camera->stopAcquisition();
	m_camera->setGain(ui->sBox_gain->value());
	m_camera->startAcquisition();
}

void DlgAddProductConfig::pbtn_spinImage_clicked()
{
	m_rotateCount++;
}

void DlgAddProductConfig::pbtn_drawRecognitionRange_clicked()
{
	m_frameSelectLabel->enableSelection(true);
}

void DlgAddProductConfig::pbt_saveProductConfig_clicked()
{
	QFileDialog 
		fileDlg(nullptr, tr("打开文件"), 
			"", tr("数据文件(*.xml);;所有文件 (*)"));
	if (fileDlg.exec() == QFileDialog::Accepted) {
		auto fileName = fileDlg.selectedFiles().first();

		ProductConfigLoader configLoader;
		configLoader.setNewFile(fileName.toStdString());

		ProductConfig config;
		config.gain = ui->sBox_gain->value();
		config.productName = ui->lEdit_productName->text().toStdString();
		config.rotateCount = m_rotateCount;
		config.ExposureTime = ui->sBox_exposureTime->value();
		config.topLeftCorner = m_recognizeRange->topLeftCorner;
		config.leftLowerCorner = m_recognizeRange->leftLowerCorner;
		config.upperRightCorner = m_recognizeRange->upperRightCorner;
		config.lowerRightCorner = m_recognizeRange->lowerRightCorner;

		auto storeConfigResult=configLoader.storeConfig(config);

		ConfigBeforeRuntimeLoader configBeforeRuntimeLoader;
		configBeforeRuntimeLoader.loadFile(m_configBeforeRuntime.toStdString());
		configBeforeRuntimeLoader.storeCameraConfig(m_camera->m_Ip, fileName.toStdString());
		configBeforeRuntimeLoader.saveFile(m_configBeforeRuntime.toStdString());

		auto saveConfigResult = configLoader.saveFile(fileName.toStdString());

		if (storeConfigResult&& saveConfigResult) {
			QMessageBox::information(this, "保存", "保存成功");
			spdlog::info("Add new product config in "+ fileName.toStdString());
		}
		else {
			QMessageBox::warning(this, "保存", "保存失败");
			spdlog::info("Failed add new product config in " + fileName.toStdString());
		}
	}
}
